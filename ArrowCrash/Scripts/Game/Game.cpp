#include "Game.h"


Pause::Pause(int numOfPlayer_, Game& gameScene)
	:numOfPlayer(numOfPlayer_),
	clickDetector(),
	targets(),
	pointers()
{
	const String font_handler = L"kokumincho";

	targets.emplace_back(new ymds::ClickableLabel(
		L"^CgÉßé", font_handler, Window::Center(), Palette::Darkslategray,
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
		//ã»?½½|ã»?½½Cã»?½½ã»?½½ã»?½½^ã»?½½?¾æ¾?½¿?½½ã»?½½ã»?½½?¾ç½®
		Point pos(Window::Center());
		const Point tmp(2 * (i % 2) - 1, 2 * (i / 2) - 1); //i == 0 ã»?½½?¾ã¨ãï½¿?½½ (-1, 0), i== ã»?½½?¾ã¨ãï½¿?½½ (0, 1)
		pos.moveBy(tmp.x * (Window::Width() / 4), tmp.y * (Window::Height() / 5));

		pointers.at(i)->setPos(pos);
	}
}


Game::Game()
	:gameData(),
	paused(false),
	pause(none),
	timer(true),
	time_limit(120),
	players()
{}

Game::~Game() {
	ymds::GamepadManager::get().inactivate();
	BlockUnitManager::clearManagerPtr();
	Field::clearFieldPtr();
	Player::clearPlayerPtr();
	ymds::EventManager::get().clear();
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

	//Q[æÊÅ\¦·éîñEBhE
	InfoWindow::setFont(L"Dash Digital-7", 0.05 * gameData.fieldSize.x);
}

void Game::update() {
  
	//E½E½E½E½E½E½E½
	if (timer.s() >= time_limit) {
		for (int i = 0; i < players.size(); i++) {
			m_data->scores.at(i) = players.at(i).getScore();
		}
		timer.pause();
		changeScene(SceneName::Result);
	}


	ymds::GamepadManager::get().update();

	static auto startClicked = []() {
		return ymds::GamepadManager::get().any([](ymds::Gamepad& gamepad) { return gamepad.clicked(ymds::GamepadIn::START); });
	};
	
	//ã»?½½|ã»?½½[ã»?½½Y
	if (paused && pause) {
		pause->update();

		if (startClicked())
		{
			paused = false;
			timer.resume();
		}
		return;
	}
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

	//èæ¯ç¨ã«ãã¬ã¤ã¤ã¼è²ãèª¿æ´
	static auto toHSV = [](const Vec3& vec) -> HSV { return HSV(vec.x, vec.y, vec.z); };
	static Color playerColorBack[4] = {

		Color(toHSV(HSV(Palette::Red).asVec3().movedBy(0.0, -0.3, 0.0))),
		Color(toHSV(HSV(Palette::Blue).asVec3().movedBy(0.0, -0.3, 0.0))),
		Color(toHSV(HSV(Palette::Yellow).asVec3().movedBy(0.0, -0.3, 0.0))),
		Color(toHSV(HSV(Palette::Green).asVec3().movedBy(0.0, -0.3, 0.1)))

	};
	
	for (int i = 0; i < m_data->numOfPlayer; i++) {
		playerPanel.at(i).draw(playerColorBack[i]);
	}

	for (auto& player : players) {
		player.draw();
	}

	ymds::EventManager::get().draw();

	//ã¿ã¤ã?è¡¨ç¤º
	int remaining = time_limit - timer.s();
	const Point timePos(Window::Center().x, Window::Height() / 25);
	FontAsset(L"digital")(remaining / 60, L":", Pad(remaining % 60, {2, L'0'})).drawAt(timePos);


	if (paused && pause) pause->draw();

}




void Game::initGameData() {

	auto&& window = Window::Size();
	uiInfo.topUIHeight = window.y / 10;

	const int numOfPlayer = m_data->numOfPlayer;

	//vC[Ìæ
	gameData.playerRegion = Size(Window::Width() / numOfPlayer, Window::Height() - uiInfo.topUIHeight);

	//tB[h
	int fieldWidth = 0;
	switch (m_data->numOfPlayer)
	{
	case 2: fieldWidth = gameData.playerRegion.x * 9 / 20; break;
	case 3:	fieldWidth = gameData.playerRegion.x * 13 / 20; break;
	case 4:	fieldWidth = gameData.playerRegion.x * 17 / 20; break;
	default: break;
	}

	//ã»?½½uã»?½½ã»?½½ã»?½½bã»?½½Nã»?½½?¾ãµã»?½½Cã»?½½Y
	int blockSize = fieldWidth / constants::col_len;
	Block::blockSize = blockSize;

	//tB[hÌTCY
	gameData.fieldSize.x = fieldWidth;
	gameData.fieldSize.y = blockSize * constants::row_len;
	//E½tE½BE½[E½E½E½hE½E½E½E½E½Ì}E½[E½WE½E½
	auto&& gauge = TextureAsset(L"gauge");
	const int ojamaWidth = blockSize*(constants::row_len - 1)*(double)gauge.width / gauge.height; //¨×Q[WÌª
	uiInfo.fieldLeftMargin = (gameData.playerRegion.x + ojamaWidth - gameData.fieldSize.x) / 2;
	//tB[hã¤Ì}[W
	uiInfo.fieldTopMargin =	(gameData.playerRegion.y - gameData.fieldSize.y) * 3 / 5;

	for (int i = 0; i < numOfPlayer; i++) {
		//E½eE½vE½E½E½CE½E½E½[E½tE½BE½[E½E½E½hE½ÌîE½_
		gameData.stdPositions.emplace_back(gameData.playerRegion.x*i + uiInfo.fieldLeftMargin, uiInfo.topUIHeight + uiInfo.fieldTopMargin);
	}

}

void Game::initUIComponents() {

	auto&& window = Window::Size();
	
	for (int i = 0; i < m_data->numOfPlayer; i++) {
		playerPanel.emplace_back(
			Point(gameData.playerRegion.x*i, uiInfo.topUIHeight), gameData.playerRegion
		);
	}

	for (int i = 0; i < m_data->numOfPlayer; i++) {
		//ã¦ããããã¬ã¼ã?ã®ãµã¤ãº
		const int unitFrameSize = Block::blockSize * 3;
		//ã¦ããããã¬ã¼ã?ã®éé
		const int unitFrameInterval = Block::blockSize * 2;
		//ã¹ããã¯æ?
		gameData.stockFrames.emplace_back(
			gameData.playerRegion.x * i + uiInfo.fieldLeftMargin,
			uiInfo.topUIHeight + uiInfo.fieldTopMargin - unitFrameSize - unitFrameInterval,
			unitFrameSize,
			unitFrameSize
		);

		//æ¬¡ã¦ãããæ 
		gameData.nextUnitFrames.emplace_back();
		for (int j = 0; j < constants::numOfNextBlocks; j++) {
			gameData.nextUnitFrames.at(i).emplace_back(
				gameData.playerRegion.x * i + uiInfo.fieldLeftMargin + gameData.fieldSize.x - unitFrameSize * (2 - j) - unitFrameSize * 1 / 3 * j,
				uiInfo.topUIHeight + uiInfo.fieldTopMargin - unitFrameSize * (j + 1) + unitFrameSize * 2 / 5 * j - unitFrameInterval,
				unitFrameSize,
				unitFrameSize
			);
		}
	}
}