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
	timer(true),
	state(State::Wait1),
	winner(),
	checked()
{
	//勝者のナンバーを取得
	int maxScore = *std::max_element(scores.begin(), scores.end());
	for (int i = 0; i < scores.size(); i++) {
		winner.emplace_back(scores[i] == maxScore);
		checked.emplace_back(false);
	}
}

void Result::update() {
	switch (state)
	{
	case Result::State::Wait1:
		if (timer.ms() >= 1300) {
			state = State::Init;
			timer.restart();
		}
		break;

	case Result::State::Init:
		backColor.a += 3;
		if (backColor.a >= 150) {
			backColor.a = 150;
			state = State::Wait2;
			timer.restart();
		}
		break;

	case Result::State::Wait2:
		if (timer.ms() >= 1000) {
			state = State::Show;
			timer.restart();
		}
		break;

	case Result::State::Show:
	{
		for (int i = 0; i < checked.size(); i++) {
			static auto twoClicked = [](int num) { return ymds::GamepadManager::get().getGamepad(num).clicked(ymds::GamepadIn::TWO); };
			if (!checked[i] && twoClicked(i)) {
				checked[i] = true;
				SoundAsset(L"hit").playMulti();
			}
		}
		//全てがtrueなら
		if (!std::any_of(checked.begin(), checked.end(), [](bool f) { return !f; })) {
			gameScene.transit(SceneName::Title);
		}
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
			const Point pos(data.playerRegion.x * i + data.playerRegion.x / 2, data.fieldSize.y);
			const Color color(checked[i] ? Color(100,100,100) : Palette::White);
			if (winner[i]) TextureAsset(L"WIN").scale(scale).drawAt(pos, color);
			else TextureAsset(L"LOSE").scale(scale).drawAt(pos, color);
		}

		Color guideColor = (timer.ms() / 500) % 2 == 0 ? Palette::White : Palette::Darkgray;
		FontAsset(L"digital")(L"press 2 key").drawAt(Window::Center().movedBy(0, Window::Height()/4), guideColor);
	}
}


Runner::Runner(String text_)
	:text(text_),
	speed((float)Window::Width() / 180.0f),
	font_handler(L"runner-font"),
	size(FontAsset(font_handler)(text).region().size),
	pos(Window::Width(), Window::Height()/2 - size.y / 2),
	x_pos(pos.x)
{}

void Runner::update() {

	//終了判定
	if (pos.x + size.x < 0) kill();

	//移動
	x_pos -= speed;
	pos.x = x_pos;
}

void Runner::draw() const {
	FontAsset(font_handler)(text).draw(pos, Palette::White);
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
	SoundAsset(L"bgm").stop();
}

void Game::init() {

	SoundAsset(L"bgm").setVolume(1.0);
	SoundAsset(L"bgm").play();

	ymds::GamepadManager::get().activate();

	initGameData();

	pause.emplace(m_data->numOfPlayer, *this);

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

	FontAsset::Register(L"runner-font", Window::Height() / 20);
}

void Game::update() {
  
	ymds::GamepadManager::get().update();

	if (timeUp) {
		result->update();
		return;
	}

	//タイムアップ
	if (timer.s() >= time_limit || Input::KeyEnter.clicked) {
		for (int i = 0; i < players.size(); i++) {
			m_data->scores.at(i) = players.at(i).getScore();
		}
		timer.pause();

		result.emplace(m_data->numOfPlayer, gameData, *this, m_data->scores);
		timeUp = true;
		SoundAsset(L"whistle").play();
		SoundAsset(L"bgm").setVolume(0.3);
	}
	//十秒前
	if (!issued10sBeforeRunner && time_limit - timer.s() <= 12) {
		ymds::EventManager::get().registerEvent(new Runner(L"残り10秒！"));
		issued10sBeforeRunner = true;
		SoundAsset(L"whistle_single").play();
	}
	//一分前
	if (!issued1mBeforeRunner && time_limit - timer.s() <= 60) {
		ymds::EventManager::get().registerEvent(new Runner(L"残り1分！"));
		issued1mBeforeRunner = true;
		SoundAsset(L"whistle_single").play();
	}

	static auto startClicked = []() {
		return ymds::GamepadManager::get().any([](ymds::Gamepad& gamepad) { return gamepad.clicked(ymds::GamepadIn::START); });
	};
	
	//ポーズ
	if (paused && pause) {
		pause->update();

		if (startClicked())
		{
			paused = false;
			timer.resume();
			SoundAsset(L"select").playMulti();
			SoundAsset(L"bgm").setVolume(1.0);
		}
		return;
	}
	if (startClicked())
	{
		paused = true;
		timer.pause();
		pause->resetPointerPos();
		SoundAsset(L"select").playMulti();
		SoundAsset(L"bgm").setVolume(0.4);
	}
	
	for (auto& player : players) {
		player.update();
	}

	ymds::EventManager::get().update();
}

void Game::draw() const {

	//背景色
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

	//残りタイム表示
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

	//ブロックサイズ
	int blockSize = fieldWidth / constants::col_len;
	Block::blockSize = blockSize;

	//フィールドのサイズ
	gameData.fieldSize.x = fieldWidth;
	gameData.fieldSize.y = blockSize * constants::row_len;
	//フィールド左部のマージン
	auto&& gauge = TextureAsset(L"gauge");
	const int ojamaWidth = blockSize*(constants::row_len - 1)*(double)gauge.width / gauge.height; //お邪魔ゲージの分
	uiInfo.fieldLeftMargin = (gameData.playerRegion.x + ojamaWidth - gameData.fieldSize.x) / 2;
	//フィールド上側のマージン
	uiInfo.fieldTopMargin =	(gameData.playerRegion.y - gameData.fieldSize.y) * 3 / 5;

	for (int i = 0; i < numOfPlayer; i++) {
		//フィールド基準点
		gameData.stdPositions.emplace_back(gameData.playerRegion.x*i + uiInfo.fieldLeftMargin, uiInfo.topUIHeight + uiInfo.fieldTopMargin);
	}

	for (int i = 0; i < m_data->numOfPlayer; i++) {
		playerPanel.emplace_back(
			Point(gameData.playerRegion.x*i, uiInfo.topUIHeight), gameData.playerRegion
		);
	}

	for (int i = 0; i < m_data->numOfPlayer; i++) {
		//ユニットフレームのサイズ
		const int unitFrameSize = Block::blockSize * 3;
		//ユニットフレームの間隔
		const int unitFrameInterval = Block::blockSize * 2;

		//ストック
		gameData.stockFrames.emplace_back(
			gameData.playerRegion.x * i + uiInfo.fieldLeftMargin,
			uiInfo.topUIHeight + uiInfo.fieldTopMargin - unitFrameSize - unitFrameInterval,
			unitFrameSize,
			unitFrameSize
		);
		//次ユニット
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