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
		//繝ｻ?ｽｽ|繝ｻ?ｽｽC繝ｻ?ｽｽ繝ｻ?ｽｽ繝ｻ?ｽｽ^繝ｻ?ｽｽ?ｾ梧得?ｽｿ?ｽｽ繝ｻ?ｽｽ繝ｻ?ｽｽ?ｾ顔ｽｮ
		Point pos(Window::Center());
		const Point tmp(2 * (i % 2) - 1, 2 * (i / 2) - 1); //i == 0 繝ｻ?ｽｽ?ｾ後→繧托ｽｿ?ｽｽ (-1, 0), i== 繝ｻ?ｽｽ?ｾ後→繧托ｽｿ?ｽｽ (0, 1)
		pos.moveBy(tmp.x * (Window::Width() / 4), tmp.y * (Window::Height() / 5));

		pointers.at(i)->setPos(pos);
	}
}


Result::Result(int numOfPlayer_, GameData& data_, Game& gameScene_, std::vector<int>& scores_)
	:numOfPlayer(numOfPlayer_),
	data(data_),
	gameScene(gameScene_),
	scores(scores_),
	backColor(Palette::Black, 0),
	timer(false),
	state(State::Init),
	winner()
{
	//勝者のナンバーを取得
	int maxScore = *std::max_element(scores.begin(), scores.end());
	for (int i = 0; i < scores.size(); i++) {
		winner.emplace_back(scores[i] == maxScore);
	}
}

void Result::update() {
	switch (state)
	{
	case Result::State::Init:
		backColor.a += 3;
		if (backColor.a >= 150) {
			backColor.a = 150;
			state = State::Wait;
			timer.start();
		}
		break;

	case Result::State::Wait:
		if (timer.ms() >= 1000) state = State::Show;
		break;

	case Result::State::Show:
	{
		bool twoClicked = ymds::GamepadManager::get().any([](ymds::Gamepad& pad) { return pad.clicked(ymds::GamepadIn::TWO); });
		if (twoClicked) gameScene.transit(SceneName::Title);
		break;
	}
	
	}
	
}

void Result::draw() const {
	static const Rect&& clientRect(Window::ClientRect());
	clientRect.draw(backColor);

	const double scale = (double)data.fieldSize.x / TextureAsset(L"WIN").width;

	if (state == State::Show) {
		for (int i = 0; i < winner.size(); i++) {
			const Point pos(data.stdPositions[i].movedBy(0, data.fieldSize.y / 5));
			if (winner[i]) TextureAsset(L"WIN").scale(scale).draw(pos);
			else TextureAsset(L"LOSE").scale(scale).draw(pos);
		}
	}
}



Game::Game()
	:gameData(),
	paused(false),
	pause(none),
	result(none),
	timeUp(false),
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

	//ゲーム画面で表示する情報ウィンドウ
	InfoWindow::setFont(L"Dash Digital-7", 0.05 * gameData.fieldSize.x);
}

void Game::update() {
  
	if (timeUp) {
		result->update();
		return;
	}

	//・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
	if (timer.s() >= time_limit || Input::KeyEnter.clicked) {
		for (int i = 0; i < players.size(); i++) {
			m_data->scores.at(i) = players.at(i).getScore();
		}
		timer.pause();

		result.emplace(m_data->numOfPlayer, gameData, *this, m_data->scores);
		timeUp = true;
	}


	ymds::GamepadManager::get().update();

	static auto startClicked = []() {
		return ymds::GamepadManager::get().any([](ymds::Gamepad& gamepad) { return gamepad.clicked(ymds::GamepadIn::START); });
	};
	
	//繝ｻ?ｽｽ|繝ｻ?ｽｽ[繝ｻ?ｽｽY
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
	
	for (auto& player : players) {
		player.update();
	}

	ymds::EventManager::get().update();
}

void Game::draw() const {

	//閭梧勹逕ｨ縺ｫ繝励Ξ繧､繝､繝ｼ濶ｲ繧定ｪｿ謨ｴ
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

	//繧ｿ繧､繝?陦ｨ遉ｺ
	int remaining = time_limit - timer.s();
	const Point timePos(Window::Center().x, Window::Height() / 25);
	FontAsset(L"digital")(remaining / 60, L":", Pad(remaining % 60, {2, L'0'})).drawAt(timePos);


	if (paused && pause) pause->draw();

	if (timeUp) result->draw();

}




void Game::initGameData() {

	auto&& window = Window::Size();
	uiInfo.topUIHeight = window.y / 10;

	const int numOfPlayer = m_data->numOfPlayer;

	//プレイヤー領域
	gameData.playerRegion = Size(Window::Width() / numOfPlayer, Window::Height() - uiInfo.topUIHeight);

	//フィールド幅
	int fieldWidth = 0;
	switch (m_data->numOfPlayer)
	{
	case 2: fieldWidth = gameData.playerRegion.x * 9 / 20; break;
	case 3:	fieldWidth = gameData.playerRegion.x * 13 / 20; break;
	case 4:	fieldWidth = gameData.playerRegion.x * 17 / 20; break;
	default: break;
	}

	//繝ｻ?ｽｽu繝ｻ?ｽｽ繝ｻ?ｽｽ繝ｻ?ｽｽb繝ｻ?ｽｽN繝ｻ?ｽｽ?ｾ後し繝ｻ?ｽｽC繝ｻ?ｽｽY
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
	
	for (int i = 0; i < m_data->numOfPlayer; i++) {
		playerPanel.emplace_back(
			Point(gameData.playerRegion.x*i, uiInfo.topUIHeight), gameData.playerRegion
		);
	}

	for (int i = 0; i < m_data->numOfPlayer; i++) {
		//繝ｦ繝九ャ繝医ヵ繝ｬ繝ｼ繝?縺ｮ繧ｵ繧､繧ｺ
		const int unitFrameSize = Block::blockSize * 3;
		//繝ｦ繝九ャ繝医ヵ繝ｬ繝ｼ繝?縺ｮ髢馴囈
		const int unitFrameInterval = Block::blockSize * 2;
		//繧ｹ繝医ャ繧ｯ譫?
		gameData.stockFrames.emplace_back(
			gameData.playerRegion.x * i + uiInfo.fieldLeftMargin,
			uiInfo.topUIHeight + uiInfo.fieldTopMargin - unitFrameSize - unitFrameInterval,
			unitFrameSize,
			unitFrameSize
		);

		//谺｡繝ｦ繝九ャ繝域棧
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