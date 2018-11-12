#include "Game.h"


Pause::Pause(int numOfPlayer_, Game& gameScene)
	:numOfPlayer(numOfPlayer_),
	clickDetector(),
	targets(),
	pointers()
{
	const String font_handler = L"kokumincho";

	targets.emplace_back(new ymds::ClickableLabel(
		L"�^�C�g���ɖ߂�", font_handler, Window::Center(), Palette::Darkslategray,
		[&gameScene](ymds::ClickableLabel& label) { gameScene.transit(SceneName::Title); },
		[](ymds::ClickableLabel& label) { label.setColor(Palette::White); },
		[](ymds::ClickableLabel& label) { label.setColor(Palette::Darkslategray); }
	));

	for (int i = 0; i < numOfPlayer; i++) {
		pointers.emplace_back(new Pointer(i, { 0, 0 }));
	}
	resetPointerPos();

	for (auto&& target : targets) clickDetector.addTarget(target);
	for (auto&& pointer : pointers) clickDetector.addPointer(pointer);
}

void Pause::update() {
	for (auto& pointer : pointers) pointer->update();
	clickDetector.update();
}

void Pause::draw() const {
	static const Texture white(Image(Window::Size(), Color(Palette::White, 150)));
	white.draw();
	for (const auto& target : targets) target->draw();
	for (const auto& pointer : pointers) pointer->draw();
}

void Pause::resetPointerPos() {
	for (int i = 0; i < numOfPlayer; i++) {
		//�|�C���^�̏���ʒu
		Point pos(Window::Center());
		const Point tmp(2 * (i % 2) - 1, 2 * (i / 2) - 1); //i == 0 �̂Ƃ� (-1, 0), i== �̂Ƃ� (0, 1)
		pos.moveBy(tmp.x * (Window::Width() / 4), tmp.y * (Window::Height() / 5));

		pointers.at(i)->setPos(pos);
	}
}


Game::Game()
	:gameData(),
	paused(false),
	pause(none),
	timer(true),
	time_limit(180),
	players()
{
	Graphics::SetBackground(Color(245, 28, 0));
}

Game::~Game() {
	ymds::GamepadManager::get().inactivate();
	BlockUnitManager::clearManagerPtr();
}

void Game::init() {
	
	ymds::GamepadManager::get().activate();

	initGameData();
	pause.emplace(m_data->numOfPlayer, *this);
	initUIComponents();

	for (int i = 0; i < m_data->numOfPlayer; i++) {
		players.emplace_back(i, gameData);
	}

	for (auto& player : players) {
		player.init();
	}

	m_data->scores.clear();
	for (int i = 0; i < m_data->numOfPlayer; i++)
		m_data->scores.emplace_back();
}

void Game::update() {
  
	//�������
	if (timer.s() > time_limit) {
		for (int i = 0; i < players.size(); i++) {
			m_data->scores.at(i) = players.at(i).getScore();
		}
		changeScene(SceneName::Result);
	}


	ymds::GamepadManager::get().update();

	static auto startClicked = []() {
		return ymds::GamepadManager::get().any([](ymds::Gamepad& gamepad) { return gamepad.clicked(ymds::GamepadIn::START); });
	};
	
	//�|�[�Y
	if (paused && pause) {
		pause->update();

		//if (Input::KeyP.clicked)
		if (startClicked())
		{
			paused = false;
			timer.resume();
		}
		return;
	}
	//if (Input::KeyP.clicked)
	if (startClicked())
	{
		paused = true;
		timer.pause();
		pause->resetPointerPos();
	}

	if (Input::KeyEnter.clicked) changeScene(SceneName::Result);

	for (auto& player : players) {
		player.update();
	}

	ymds::EventManager::get().update();
}

void Game::draw() const {

	for (auto& player : players) {
		player.draw();
	}

	uiComp.draw();

	ymds::EventManager::get().draw();

	PutText(time_limit - timer.s()).at(Window::Center().x, 20);


	if (paused && pause) pause->draw();

}




void Game::initGameData() {

	auto&& window = Window::Size();
	uiInfo.topUIHeight = window.y / 10;

	const int numOfPlayer = m_data->numOfPlayer;

	//�e�v���C���[�G���A�̃T�C�Y
	uiInfo.playerRegion = Size(Window::Width() / numOfPlayer, Window::Height() - uiInfo.topUIHeight);
	
	//�t�B�[���h��
	int fieldWidth = 0;
	switch (m_data->numOfPlayer)
	{
	case 2: fieldWidth = uiInfo.playerRegion.x * 2 / 5; break;
	case 3:	fieldWidth = uiInfo.playerRegion.x / 2; break;
	case 4:	fieldWidth = uiInfo.playerRegion.x * 3 / 5;	break;
	default: break;
	}

	//�u���b�N�̃T�C�Y
	int blockSize = fieldWidth / constants::col_len;
	Block::blockSize = blockSize;
	//���j�b�g�t���[��(�X�g�b�N�A���u���b�N�g)�̃T�C�Y
	uiInfo.unitFrameSize = blockSize * 4;
	//フィールドのサイズ
	uiInfo.fieldSize.x = fieldWidth;
	uiInfo.fieldSize.y = blockSize * constants::row_len;
	//�t�B�[���h�����̃}�[�W��
	uiInfo.fieldLeftMargin = (uiInfo.playerRegion.x - uiInfo.fieldSize.x) / 2;
	//�t�B�[���h�㑤�̃}�[�W��
	uiInfo.fieldTopMargin =
		numOfPlayer == 2 ?
		(uiInfo.playerRegion.y - uiInfo.fieldSize.y) / 3 :
		(uiInfo.playerRegion.y - uiInfo.fieldSize.y) / 2;

	for (int i = 0; i < numOfPlayer; i++) {
		//�e�v���C���[�t�B�[���h�̊�_
		gameData.stdPositions.emplace_back(uiInfo.playerRegion.x*i + uiInfo.fieldLeftMargin, uiInfo.topUIHeight + uiInfo.fieldTopMargin);
	}

}

void Game::initUIComponents() {

	auto&& window = Window::Size();
	uiComp.topUIBorder.set({ 0, uiInfo.topUIHeight }, { window.x, uiInfo.topUIHeight });

	for (int i = 1; i < m_data->numOfPlayer; i++) {
		uiComp.playerBorders.emplace_back(
			i*uiInfo.playerRegion.x, uiInfo.topUIHeight,
			i*uiInfo.playerRegion.x, window.y
		);
	}
	
	switch (int numOfPlayer = m_data->numOfPlayer)
	{
	case 2:
		for (int i = 0; i < numOfPlayer; i++) {
			//���j�b�g�t���[���̊Ԋu
			const int unitFrameInterval = Block::blockSize / 2;
			//�X�g�b�N�g
			uiComp.stockFrames.emplace_back(
				uiInfo.playerRegion.x * i + uiInfo.fieldLeftMargin - uiInfo.unitFrameSize - unitFrameInterval,
				uiInfo.topUIHeight + uiInfo.fieldTopMargin,
				uiInfo.unitFrameSize,
				uiInfo.unitFrameSize
				);

			//�����j�b�g�g(���Ԃ͓K��)
			uiComp.nextUnitFrames.emplace_back();
			for (int j = 0; j < constants::numOfNextBlocks; j++) {
				uiComp.nextUnitFrames.at(i).emplace_back(
					uiInfo.playerRegion.x * i + uiInfo.fieldLeftMargin + uiInfo.fieldSize.x + unitFrameInterval,
					uiInfo.topUIHeight + uiInfo.fieldTopMargin + uiInfo.unitFrameSize * j,
					uiInfo.unitFrameSize,
					uiInfo.unitFrameSize
				);
			}
		}
		gameData.nextUnitFrames = &uiComp.nextUnitFrames;
		gameData.stockFrames = &uiComp.stockFrames;
		break;
	case 3:
	case 4:
		for (int i = 0; i < numOfPlayer; i++) {
			//���j�b�g�t���[���̊Ԋu
			const int unitFrameInterval = Block::blockSize * 2;
			//�X�g�b�N�g
			uiComp.stockFrames.emplace_back(
				uiInfo.playerRegion.x * i + uiInfo.fieldLeftMargin,
				uiInfo.topUIHeight + uiInfo.fieldTopMargin - uiInfo.unitFrameSize - unitFrameInterval,
				uiInfo.unitFrameSize,
				uiInfo.unitFrameSize
			);

			//�����j�b�g�g(���Ԃ͓K��)
			uiComp.nextUnitFrames.emplace_back();
			for (int j = 0; j < constants::numOfNextBlocks; j++) {
				uiComp.nextUnitFrames.at(i).emplace_back(
					uiInfo.playerRegion.x * i + uiInfo.fieldLeftMargin + uiInfo.fieldSize.x - uiInfo.unitFrameSize,
					uiInfo.topUIHeight + uiInfo.fieldTopMargin - uiInfo.unitFrameSize * (j+1) - unitFrameInterval,
					uiInfo.unitFrameSize,
					uiInfo.unitFrameSize
				);
			}
		}
	}
	gameData.nextUnitFrames = &uiComp.nextUnitFrames;
	gameData.stockFrames = &uiComp.stockFrames;
}


void Game::UIComponents::draw() const {

	topUIBorder.draw();
	
	for (const auto& line : playerBorders) {
		line.draw();
	}
	
	for (const auto& stockFrame : stockFrames) {
		stockFrame.drawFrame();
	}
	
	/*for (const auto& arr : nextUnitFrames) {
		for (const auto& nextUnitFrame : arr) {
			nextUnitFrame.drawFrame();
		}
	}*/
}