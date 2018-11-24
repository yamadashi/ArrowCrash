#include "Title.h"

Title::Title()
	:Scene(),
	clickDetector(),
	pointers(),
	targets(),
	scene(TitleScene::TOP),
	transition(false),
	maxSpeed(Window::Width()/25),
	speed(maxSpeed),
	deceleration((double)maxSpeed/60),
	selectViewPos({ Window::Width(), 0 }),
	alpha(0)
{
	Graphics::SetBackground(Palette::Black);
	SoundAsset(L"title_bgm").play();
}

void Title::init() {

	ymds::GamepadManager::get().activate();

	//pointer
	for (int i = 0; i < 4; i++) {
		//ポインタの初期位置
		Point pos(Window::Center().movedBy(0, Window::Height() / 6));
		const Point tmp(2 * (i % 2) - 1, 2 * (i / 2) - 1); //i == 0 のとき (-1, 0), i== のとき (0, 1)
		pos.moveBy(0.2 * Window::Width() * tmp.x, 0.2 * tmp.y * Window::Height());

		pointers.emplace_back(new Pointer(i, pos));
	}


	Size titleTextureSize = TextureAsset(L"title").size;
	double scale = (Window::Height() / 3.0) / titleTextureSize.y;
	Size scaledTitleSize(titleTextureSize.x * scale, titleTextureSize.y * scale);
	Point titleOffset(0, -Window::Height() / 6);
	
	targets.emplace_back(new ymds::ClickablePanel(L"title", Window::Center().movedBy(-scaledTitleSize / 2).movedBy(titleOffset), scaledTitleSize));


	const String font_handler = L"kokumincho";
	const int labelInterval = Window::Height() / 36;
	const int labelHeight = FontAsset(font_handler).height;
	const int labelOffset = Window::Height() / 10;

	targets.emplace_back(new ymds::ClickableLabel(L"はじめる", font_handler, Window::Center().movedBy(0, labelOffset), Palette::White,
		[this](ymds::ClickableLabel&) { SoundAsset(L"select").playMulti(); transition = true; },
		[](ymds::ClickableLabel& label) { label.setColor(Palette::Darkslategray); },
		[](ymds::ClickableLabel& label) { label.setColor(Palette::White); }
	));
	targets.emplace_back(new ymds::ClickableLabel(L"せつめい", font_handler, Window::Center().movedBy(0, labelOffset + labelHeight + labelInterval), Palette::White,
		[this](ymds::ClickableLabel&) { SoundAsset(L"select").playMulti(); changeScene(SceneName::Explain); },
		[](ymds::ClickableLabel& label) { label.setColor(Palette::Darkslategray); },
		[](ymds::ClickableLabel& label) { label.setColor(Palette::White); }
	));
	targets.emplace_back(new ymds::ClickableLabel(L"おわる", font_handler, Window::Center().movedBy(0, labelOffset + 2 * (labelHeight + labelInterval)), Palette::White,
		[this](ymds::ClickableLabel&) { System::Exit(); },
		[](ymds::ClickableLabel& label) { label.setColor(Palette::Darkslategray); },
		[](ymds::ClickableLabel& label) { label.setColor(Palette::White); }
	));


	const int panelInterval = Window::Height() / 36;
	const int panelSize = Window::Width() / 5;
	const int panelLeft = (Window::Width() - panelSize * 3 - panelInterval * 2) / 2;
	const int panelOver = (Window::Height() - panelSize) / 2;
	const int backButtonMargin = Window::Height() / 54;
	const int backButtonSize = Window::Height() / 5;

	targets.emplace_back(new ymds::ClickablePanel(L"2PlayerPanel", selectViewPos.movedBy(panelLeft, panelOver), Size(panelSize, panelSize),
		[this](ymds::ClickablePanel&) { 
		SoundAsset(L"select").playMulti();
		m_data->numOfPlayer = 2;
		changeScene(SceneName::Game);
		SoundAsset(L"title_bgm").stop(); },
		[](ymds::ClickablePanel& panel) { panel.setTextureHandler(L"2PlayerPanel_"); },
		[](ymds::ClickablePanel& panel) { panel.setTextureHandler(L"2PlayerPanel"); }
	));
	targets.emplace_back(new ymds::ClickablePanel(L"3PlayerPanel", selectViewPos.movedBy(panelLeft + panelSize + panelInterval, panelOver), Size(panelSize, panelSize),
		[this](ymds::ClickablePanel&) {
		SoundAsset(L"select").playMulti();
		m_data->numOfPlayer = 3;
		changeScene(SceneName::Game);
		SoundAsset(L"title_bgm").stop(); },
		[](ymds::ClickablePanel& panel) { panel.setTextureHandler(L"3PlayerPanel_"); },
		[](ymds::ClickablePanel& panel) { panel.setTextureHandler(L"3PlayerPanel"); }
	));
	targets.emplace_back(new ymds::ClickablePanel(L"4PlayerPanel", selectViewPos.movedBy(panelLeft + panelSize * 2 + panelInterval * 2, panelOver), Size(panelSize, panelSize),
		[this](ymds::ClickablePanel&) {
		SoundAsset(L"select").playMulti();
		m_data->numOfPlayer = 4;
		changeScene(SceneName::Game);
		SoundAsset(L"title_bgm").stop(); },
		[](ymds::ClickablePanel& panel) { panel.setTextureHandler(L"4PlayerPanel_"); },
		[](ymds::ClickablePanel& panel) { panel.setTextureHandler(L"4PlayerPanel"); }
	));
	targets.emplace_back(new ymds::ClickablePanel(L"back", selectViewPos.movedBy(backButtonMargin, backButtonMargin), Size(backButtonSize, backButtonSize),
		[this](ymds::ClickablePanel&) { SoundAsset(L"select").playMulti(); transition = true; }
	));


	for (auto& target : targets) {
		clickDetector.addTarget(target);
	}
	for (auto& pointer : pointers) {
		clickDetector.addPointer(pointer);
	}
}

Title::~Title() {
	ymds::GamepadManager::get().inactivate();
}

void Title::update() {
  	
	ymds::GamepadManager::get().update();
	
	for (auto& pointer : pointers) pointer->update();

	if (!transition) {
		clickDetector.update(); 
	}
	else {
		//加速
		speed -= deceleration;

		//遷移
		int direction = scene == TitleScene::TOP ? -1 : 1;
		for (auto&& target : targets) {
			target->moveBy({ direction*speed, 0 });
		}
		selectViewPos.moveBy({ direction*speed, 0 });

		alpha += 3 * -direction;

		if (scene == TitleScene::TOP) {
			if (selectViewPos.x <= 0) {
				selectViewPos.x = 0;
				speed = maxSpeed;
				transition = false;
				scene = TitleScene::SELECT;
			}
		}
		else {
			if (selectViewPos.x >= Window::Width()) {
				selectViewPos.x = Window::Width();
				speed = maxSpeed;
				transition = false;
				scene = TitleScene::TOP;
				alpha = 0;
			}
		}
	}
}

void Title::draw() const {

	TextureAsset(L"background").resize(Window::Size()).draw();

	static const Rect clientRect(0, 0, Window::Size());
	clientRect.draw(Color(Palette::White, alpha));

	for (const auto& target : targets) target->draw();

	for (const auto& pointer : pointers) pointer->draw();
}