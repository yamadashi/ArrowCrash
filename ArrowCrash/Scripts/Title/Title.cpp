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
	backgroundPos(),
	backScrollSpeed(5)
{
	Graphics::SetBackground(Palette::Black);
}

void Title::init() {
	backgroundPos[0].set(0, 0);
	backgroundPos[1].set(Window::Width(), 0);

	ymds::GamepadManager::get().activate();

	//pointer
	for (int i = 0; i < 4; i++) {
		//É|ÉCÉìÉ^ÇÃèâä˙à íu
		Point pos(Window::Center().movedBy(0, Window::Height() / 6));
		const Point tmp(2 * (i % 2) - 1, 2 * (i / 2) - 1); //i == 0 ÇÃÇ∆Ç´ (-1, 0), i== ÇÃÇ∆Ç´ (0, 1)
		pos.moveBy(tmp.x * (Window::Width() / 4), tmp.y * (Window::Height() / 5));

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

	targets.emplace_back(new ymds::ClickableLabel(L"ÇÕÇ∂ÇﬂÇÈ", font_handler, Window::Center().movedBy(0, labelOffset), Palette::Darkslategray,
		[this](ymds::ClickableLabel&) { transition = true; },
		[](ymds::ClickableLabel& label) { label.setColor(Palette::White); },
		[](ymds::ClickableLabel& label) { label.setColor(Palette::Darkslategray); }
	));
	targets.emplace_back(new ymds::ClickableLabel(L"ÇπÇ¬ÇﬂÇ¢", font_handler, Window::Center().movedBy(0, labelOffset + labelHeight + labelInterval), Palette::Darkslategray,
		[this](ymds::ClickableLabel&) { changeScene(SceneName::Explain); },
		[](ymds::ClickableLabel& label) { label.setColor(Palette::White); },
		[](ymds::ClickableLabel& label) { label.setColor(Palette::Darkslategray); }
	));
	targets.emplace_back(new ymds::ClickableLabel(L"Ç®ÇÌÇÈ", font_handler, Window::Center().movedBy(0, labelOffset + 2 * (labelHeight + labelInterval)), Palette::Darkslategray,
		[this](ymds::ClickableLabel&) { System::Exit(); },
		[](ymds::ClickableLabel& label) { label.setColor(Palette::White); },
		[](ymds::ClickableLabel& label) { label.setColor(Palette::Darkslategray); }
	));


	const int panelInterval = Window::Height() / 36;
	const int panelSize = Window::Width() / 5;
	const int panelLeft = (Window::Width() - panelSize * 3 - panelInterval * 2) / 2;
	const int panelOver = (Window::Height() - panelSize) / 2;
	const int backButtonMargin = Window::Height() / 54;
	const int backButtonSize = Window::Height() / 5;

	targets.emplace_back(new ymds::ClickablePanel(L"2PlayerPanel", selectViewPos.movedBy(panelLeft, panelOver), Size(panelSize, panelSize),
		[this](ymds::ClickablePanel&) { m_data->numOfPlayer = 2; changeScene(SceneName::Game); },
		[](ymds::ClickablePanel& panel) { panel.setTextureHandler(L"2PlayerPanel_"); },
		[](ymds::ClickablePanel& panel) { panel.setTextureHandler(L"2PlayerPanel"); }
	));
	targets.emplace_back(new ymds::ClickablePanel(L"3PlayerPanel", selectViewPos.movedBy(panelLeft + panelSize + panelInterval, panelOver), Size(panelSize, panelSize),
		[this](ymds::ClickablePanel&) { m_data->numOfPlayer = 3; changeScene(SceneName::Game); },
		[](ymds::ClickablePanel& panel) { panel.setTextureHandler(L"3PlayerPanel_"); },
		[](ymds::ClickablePanel& panel) { panel.setTextureHandler(L"3PlayerPanel"); }
	));
	targets.emplace_back(new ymds::ClickablePanel(L"4PlayerPanel", selectViewPos.movedBy(panelLeft + panelSize * 2 + panelInterval * 2, panelOver), Size(panelSize, panelSize),
		[this](ymds::ClickablePanel&) { m_data->numOfPlayer = 4; changeScene(SceneName::Game); },
		[](ymds::ClickablePanel& panel) { panel.setTextureHandler(L"4PlayerPanel_"); },
		[](ymds::ClickablePanel& panel) { panel.setTextureHandler(L"4PlayerPanel"); }
	));
	targets.emplace_back(new ymds::ClickablePanel(L"back", selectViewPos.movedBy(backButtonMargin, backButtonMargin), Size(backButtonSize, backButtonSize),
		[this](ymds::ClickablePanel&) { transition = true; }
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
  
	//îwåi
	for (int i = 0; i < backgroundPos.size(); i++) {
		if (backgroundPos[i].x < -Window::Width()) {
			backgroundPos[i].x = backgroundPos[(i + 1) % 2].x + Window::Width();
		}
		backgroundPos[i].moveBy(-backScrollSpeed, 0);
	}
	
	ymds::GamepadManager::get().update();
	
	for (auto& pointer : pointers) pointer->update();

	if (!transition) {
		clickDetector.update(); 
	}
	else {
		//â¡ë¨
		speed -= deceleration;

		//ëJà⁄
		int direction = scene == TitleScene::TOP ? -1 : 1;
		for (auto&& target : targets) {
			target->moveBy({ direction*speed, 0 });
		}
		selectViewPos.moveBy({ direction*speed, 0 });

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
			}
		}
	}
}

void Title::draw() const {
	for (auto& pos : backgroundPos) {
		TextureAsset(L"background").resize(Window::Size()).draw(pos);
	}

	for (const auto& target : targets) target->draw();

	for (const auto& pointer : pointers) pointer->draw();
}