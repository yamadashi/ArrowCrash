#include "Game.h"


Pause::Pause(int numOfPlayer_, Game& gameScene)
	:numOfPlayer(numOfPlayer_),
	clickDetector(),
	targets(),
	pointers()
{
	const String font_handler = L"kokumincho";

	targets.emplace_back(new ymds::ClickableLabel(
		L"タイトルに戻る", font_handler, Window::Center(), Palette::Darkslategray,
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
		//・ｽ|・ｽC・ｽ・ｽ・ｽ^・ｽﾌ擾ｿｽ・ｽ・ｽﾊ置
		Point pos(Window::Center());
		const Point tmp(2 * (i % 2) - 1, 2 * (i / 2) - 1); //i == 0 ・ｽﾌとゑｿｽ (-1, 0), i== ・ｽﾌとゑｿｽ (0, 1)
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

	//突然何って感じやけど、許して
	//ゲーム画面で表示する情報ウィンドウの
	InfoWindow::setFont(L"Dash Digital-7", 0.05 * gameData.fieldSize.x);
}

void Game::update() {
  
	//・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
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
	
	//・ｽ|・ｽ[・ｽY
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

	//背景用にプレイヤー色を調整
	static auto toHSV = [](const Vec3& vec) -> HSV { return HSV(vec.x, vec.y, vec.z); };
	static Color playerColorBack[4] = {

		Color(toHSV(HSV(Palette::Red).asVec3().movedBy(0.0, -0.3, 0.0))),
		Color(toHSV(HSV(Palette::Blue).asVec3().movedBy(0.0, -0.3, 0.0))),
		Color(toHSV(HSV(Palette::Yellow).asVec3().movedBy(0.0, -0.3, 0.0))),
		Color(toHSV(HSV(Palette::Green).asVec3().movedBy(0.0, -0.3, 0.1)))

	};
	
	for (int i = 0; i < m_data->numOfPlayer; i++) {
		uiComp.playerPanel.at(i).draw(playerColorBack[i]);
	}

	for (auto& player : players) {
		player.draw();
	}

	uiComp.draw();

	ymds::EventManager::get().draw();

	//タイム表示
	int remaining = time_limit - timer.s();
	const Point timePos(Window::Center().x, Window::Height() / 25);
	FontAsset(L"digital")(remaining / 60, L":", Pad(remaining % 60, {2, L'0'})).drawAt(timePos);


	if (paused && pause) pause->draw();

}




void Game::initGameData() {

	auto&& window = Window::Size();
	uiInfo.topUIHeight = window.y / 10;

	const int numOfPlayer = m_data->numOfPlayer;

	//プレイヤー領域
	gameData.playerRegion = Size(Window::Width() / numOfPlayer, Window::Height() - uiInfo.topUIHeight);

	//フィール幅
	int fieldWidth = 0;
	switch (m_data->numOfPlayer)
	{
	case 2: fieldWidth = gameData.playerRegion.x * 13 / 30; break;
	case 3:	fieldWidth = gameData.playerRegion.x * 3 / 5;	break;
	case 4:	fieldWidth = gameData.playerRegion.x * 7 / 10; break;
	default: break;
	}

	//・ｽu・ｽ・ｽ・ｽb・ｽN・ｽﾌサ・ｽC・ｽY
	int blockSize = fieldWidth / constants::col_len;
	Block::blockSize = blockSize;
	//フィールドのサイズ
	gameData.fieldSize.x = fieldWidth;
	gameData.fieldSize.y = blockSize * constants::row_len;
	//・ｽt・ｽB・ｽ[・ｽ・ｽ・ｽh・ｽ・ｽ・ｽ・ｽ・ｽﾌマ・ｽ[・ｽW・ｽ・ｽ
	auto&& gauge = TextureAsset(L"gauge");
	const int ojamaWidth = blockSize*(constants::row_len - 1)*(double)gauge.width / gauge.height; //お邪魔ゲージの分
	uiInfo.fieldLeftMargin = (gameData.playerRegion.x + ojamaWidth - gameData.fieldSize.x) / 2;
	//フィールド上側のマージン
	uiInfo.fieldTopMargin =	(gameData.playerRegion.y - gameData.fieldSize.y) * 3 / 5;


	for (int i = 0; i < numOfPlayer; i++) {
		//・ｽe・ｽv・ｽ・ｽ・ｽC・ｽ・ｽ・ｽ[・ｽt・ｽB・ｽ[・ｽ・ｽ・ｽh・ｽﾌ基準・ｽ_
		gameData.stdPositions.emplace_back(gameData.playerRegion.x*i + uiInfo.fieldLeftMargin, uiInfo.topUIHeight + uiInfo.fieldTopMargin);
	}

}

void Game::initUIComponents() {

	auto&& window = Window::Size();
	uiComp.topUIBorder.set({ 0, uiInfo.topUIHeight }, { window.x, uiInfo.topUIHeight });

	for (int i = 0; i < m_data->numOfPlayer; i++) {
		uiComp.playerPanel.emplace_back(
			Point(gameData.playerRegion.x*i, uiInfo.topUIHeight), gameData.playerRegion
		);
	}

	for (int i = 0; i < m_data->numOfPlayer; i++) {
		//ユニットフレームのサイズ
		const int unitFrameSize = Block::blockSize * 3;
		//ユニットフレームの間隔
		const int unitFrameInterval = Block::blockSize * 2;
		//ストック枠
		uiComp.stockFrames.emplace_back(
			gameData.playerRegion.x * i + uiInfo.fieldLeftMargin,
			uiInfo.topUIHeight + uiInfo.fieldTopMargin - unitFrameSize - unitFrameInterval,
			unitFrameSize,
			unitFrameSize
		);

		//次ユニット枠
		uiComp.nextUnitFrames.emplace_back();
		for (int j = 0; j < constants::numOfNextBlocks; j++) {
			uiComp.nextUnitFrames.at(i).emplace_back(
				gameData.playerRegion.x * i + uiInfo.fieldLeftMargin + gameData.fieldSize.x - unitFrameSize * (2 - j) - unitFrameSize * 1 / 3 * j,
				uiInfo.topUIHeight + uiInfo.fieldTopMargin - unitFrameSize * (j + 1) + unitFrameSize * 2 / 5 * j - unitFrameInterval,
				unitFrameSize,
				unitFrameSize
			);
		}
	}
	gameData.nextUnitFrames = &uiComp.nextUnitFrames;
	gameData.stockFrames = &uiComp.stockFrames;
}


//そのうちいらなくなる
void Game::UIComponents::draw() const {

	topUIBorder.draw();
	
	for (const auto& stockFrame : stockFrames) {
		stockFrame.drawFrame();
	}
	
}