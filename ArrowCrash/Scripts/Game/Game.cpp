#include "Game.h"


Game::Game()
	:gameData(),
	pause(false),
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

	//制限時間
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

	static auto selectClicked = []() {
		return ymds::GamepadManager::get().any([](ymds::Gamepad& gamepad) { return gamepad.clicked(ymds::GamepadIn::SELECT); });
	};

	//ポーズ解除
	if (pause) {
		if (startClicked())
		//if (Input::KeyP.clicked)
			pause = false;
		return;
	}

	if (startClicked())
	//if (Input::KeyP.clicked)
		pause = true;
	if (selectClicked()) {
		changeScene(SceneName::Result);
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

	if (pause) {
		static const Texture white(Image(Window::Size(), Color(Palette::White, 150)));
		white.draw();
	}

	ymds::EventManager::get().draw();

	PutText(time_limit - timer.s()).at(Window::Center().x, 20);
}




void Game::initGameData() {

	auto&& window = Window::Size();
	uiInfo.topUIHeight = window.y / 10;

	const int numOfPlayer = m_data->numOfPlayer;

	//各プレイヤーエリアのサイズ
	uiInfo.playerRegion = Size(Window::Width() / numOfPlayer, Window::Height() - uiInfo.topUIHeight);
	
	//フィールド幅
	int fieldWidth = 0;
	switch (m_data->numOfPlayer)
	{
	case 2: fieldWidth = uiInfo.playerRegion.x * 2 / 5; break;
	case 3:	fieldWidth = uiInfo.playerRegion.x / 2; break;
	case 4:	fieldWidth = uiInfo.playerRegion.x * 3 / 5;	break;
	default: break;
	}

	//ブロックのサイズ
	int blockSize = fieldWidth / constants::col_len;
	Block::blockSize = blockSize;
	//フィールドのサイズ
	uiInfo.fieldSize.x = fieldWidth;
	uiInfo.fieldSize.y = blockSize * constants::row_len;
	//フィールド左側のマージン
	uiInfo.fieldLeftMargin = (uiInfo.playerRegion.x - uiInfo.fieldSize.x) / 2;
	//フィールド上側のマージン
	uiInfo.fieldTopMargin =	(uiInfo.playerRegion.y - uiInfo.fieldSize.y) * 3 / 5;

	for (int i = 0; i < numOfPlayer; i++) {
		//各プレイヤーフィールドの基準点
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

	for (int i = 0; i < m_data->numOfPlayer; i++) {
		//ユニットフレーム(ストック、次ブロック枠)のサイズ
		const int unitFrameSize = Block::blockSize * 3;
		//ユニットフレームの間隔
		const int unitFrameInterval = Block::blockSize * 2;
		//ストック枠
		uiComp.stockFrames.emplace_back(
			uiInfo.playerRegion.x * i + uiInfo.fieldLeftMargin,
			uiInfo.topUIHeight + uiInfo.fieldTopMargin - unitFrameSize - unitFrameInterval,
			unitFrameSize,
			unitFrameSize
		);

		//次ユニット枠(順番は適当)
		uiComp.nextUnitFrames.emplace_back();
		for (int j = 0; j < constants::numOfNextBlocks; j++) {
			uiComp.nextUnitFrames.at(i).emplace_back(
				uiInfo.playerRegion.x * i + uiInfo.fieldLeftMargin + uiInfo.fieldSize.x - unitFrameSize * (2 - j) - unitFrameSize / 3 * j,
				uiInfo.topUIHeight + uiInfo.fieldTopMargin - unitFrameSize * (j + 1) + unitFrameSize * 2 / 5 * j - unitFrameInterval,
				unitFrameSize,
				unitFrameSize
			);
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