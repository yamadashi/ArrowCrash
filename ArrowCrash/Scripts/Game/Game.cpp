#include "Game.h"


Pause::Pause(int numOfPlayer_, Game& gameScene)
	:numOfPlayer(numOfPlayer_),
	clickDetector(),
	targets(),
	pointers()
{
	const String font_handler = L"kokumincho";

	targets.emplace_back(new ymds::ClickableLabel(
		L"É^ÉCÉgÉãÇ…ñﬂÇÈ", font_handler, Window::Center(), Palette::Darkslategray,
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
		//„Éª?ΩΩ|„Éª?ΩΩC„Éª?ΩΩ„Éª?ΩΩ„Éª?ΩΩ^„Éª?ΩΩ?æåÊìæ?Ωø?ΩΩ„Éª?ΩΩ„Éª?ΩΩ?æäÁΩÆ
		Point pos(Window::Center());
		const Point tmp(2 * (i % 2) - 1, 2 * (i / 2) - 1); //i == 0 „Éª?ΩΩ?æå„Å®„ÇëÔΩø?ΩΩ (-1, 0), i== „Éª?ΩΩ?æå„Å®„ÇëÔΩø?ΩΩ (0, 1)
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
}

void Game::update() {
  
	//„Éª?ΩΩ„Éª?ΩΩ„Éª?ΩΩ„Éª?ΩΩ„Éª?ΩΩ„Éª?ΩΩ„Éª?ΩΩ
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
	
	//„Éª?ΩΩ|„Éª?ΩΩ[„Éª?ΩΩY
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

	//ËÉåÊôØÁî®„Å´„Éó„É¨„Ç§„É§„ÉºËâ≤„ÇíË™øÊï¥
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

	//„Çø„Ç§„É?Ë°®Á§∫
	int remaining = time_limit - timer.s();
	const Point timePos(Window::Center().x, Window::Height() / 50);
	FontAsset(L"digital")(remaining / 60, L":", Pad(remaining % 60, {2, L'0'})).drawAt(timePos);


	if (paused && pause) pause->draw();

}




void Game::initGameData() {

	auto&& window = Window::Size();
	uiInfo.topUIHeight = window.y / 10;

	const int numOfPlayer = m_data->numOfPlayer;

	//„Éª?ΩΩe„Éª?ΩΩv„Éª?ΩΩ„Éª?ΩΩ„Éª?ΩΩC„Éª?ΩΩ„Éª?ΩΩ„Éª?ΩΩ[„Éª?ΩΩG„Éª?ΩΩ„Éª?ΩΩ„Éª?ΩΩA„Éª?ΩΩ?æå„Çµ„Éª?ΩΩC„Éª?ΩΩY
	uiInfo.playerRegion = Size(Window::Width() / numOfPlayer, Window::Height() - uiInfo.topUIHeight);
	
	//„Éª?ΩΩt„Éª?ΩΩB„Éª?ΩΩ[„Éª?ΩΩ„Éª?ΩΩ„Éª?ΩΩh„Éª?ΩΩ„Éª?ΩΩ
	int fieldWidth = 0;
	switch (m_data->numOfPlayer)
	{
	case 2: fieldWidth = uiInfo.playerRegion.x * 9 / 20; break;
	case 3:	fieldWidth = uiInfo.playerRegion.x * 13 / 20; break;
	case 4:	fieldWidth = uiInfo.playerRegion.x * 17 / 20; break;
	default: break;
	}

	//„Éª?ΩΩu„Éª?ΩΩ„Éª?ΩΩ„Éª?ΩΩb„Éª?ΩΩN„Éª?ΩΩ?æå„Çµ„Éª?ΩΩC„Éª?ΩΩY
	int blockSize = fieldWidth / constants::col_len;
	Block::blockSize = blockSize;
	//„Éï„Ç£„Éº„É´„Éâ„?Æ„Çµ„Ç§„Ç∫
	uiInfo.fieldSize.x = fieldWidth;
	uiInfo.fieldSize.y = blockSize * constants::row_len;
	//„Éª?ΩΩt„Éª?ΩΩB„Éª?ΩΩ[„Éª?ΩΩ„Éª?ΩΩ„Éª?ΩΩh„Éª?ΩΩ„Éª?ΩΩ„Éª?ΩΩ„Éª?ΩΩ„Éª?ΩΩ?æå„?û„?ª?ΩΩ[„Éª?ΩΩW„Éª?ΩΩ„Éª?ΩΩ
	uiInfo.fieldLeftMargin = (uiInfo.playerRegion.x - uiInfo.fieldSize.x) / 2;
	//„Éï„Ç£„Éº„É´„Éâ‰∏äÂ?¥„ÅÆ„Éû„?º„Ç∏„É≥
	uiInfo.fieldTopMargin =	(uiInfo.playerRegion.y - uiInfo.fieldSize.y) * 3 / 5;


	for (int i = 0; i < numOfPlayer; i++) {
		//„Éª?ΩΩe„Éª?ΩΩv„Éª?ΩΩ„Éª?ΩΩ„Éª?ΩΩC„Éª?ΩΩ„Éª?ΩΩ„Éª?ΩΩ[„Éª?ΩΩt„Éª?ΩΩB„Éª?ΩΩ[„Éª?ΩΩ„Éª?ΩΩ„Éª?ΩΩh„Éª?ΩΩ?æåÂü∫Ê∫ñ„?ª?ΩΩ_
		gameData.stdPositions.emplace_back(uiInfo.playerRegion.x*i + uiInfo.fieldLeftMargin, uiInfo.topUIHeight + uiInfo.fieldTopMargin);
	}

}

void Game::initUIComponents() {

	auto&& window = Window::Size();
	uiComp.topUIBorder.set({ 0, uiInfo.topUIHeight }, { window.x, uiInfo.topUIHeight });

	for (int i = 0; i < m_data->numOfPlayer; i++) {
		uiComp.playerPanel.emplace_back(
			Point(uiInfo.playerRegion.x*i, uiInfo.topUIHeight), uiInfo.playerRegion
		);
	}

	for (int i = 0; i < m_data->numOfPlayer; i++) {
		//„É¶„Éã„ÉÉ„Éà„Éï„É¨„Éº„É?„ÅÆ„Çµ„Ç§„Ç∫
		const int unitFrameSize = Block::blockSize * 3;
		//„É¶„Éã„ÉÉ„Éà„Éï„É¨„Éº„É?„ÅÆÈñìÈöî
		const int unitFrameInterval = Block::blockSize * 2;
		//„Çπ„Éà„ÉÉ„ÇØÊû?
		uiComp.stockFrames.emplace_back(
			uiInfo.playerRegion.x * i + uiInfo.fieldLeftMargin,
			uiInfo.topUIHeight + uiInfo.fieldTopMargin - unitFrameSize - unitFrameInterval,
			unitFrameSize,
			unitFrameSize
		);

		//Ê¨°„É¶„Éã„ÉÉ„ÉàÊû†
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


//„Åù„?Æ„Å?„Å°„Å?„Çâ„Å™„Åè„Å™„Ç?
void Game::UIComponents::draw() const {

	topUIBorder.draw();
	
	for (const auto& stockFrame : stockFrames) {
		stockFrame.drawFrame();
	}
	
}