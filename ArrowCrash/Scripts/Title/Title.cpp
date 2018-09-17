#include "Title.h"

Title::Title()
	:Scene(),
	clickDetector(),
	pointer(new Pointer(0)),
	scene(TitleScene::TOP),
	transition(false)
{
	const String font_handler = L"kokumincho30";
	const int labelInterval = Window::Height() / 36;
	const int labelHeight = FontAsset(font_handler).height;

	targets.emplace_back(new ClickableLabel(L"ArrowCrash", Window::Center().movedBy(0, -140), font_handler, Palette::Orange));
	targets.emplace_back(new ClickableLabel(L"‚Í‚¶‚ß‚é", Window::Center(), font_handler, Palette::Darkslategray,
		[this](ClickableLabel&) { transition = true; },
		[](ClickableLabel& label) { label.setColor(Palette::White); },
		[](ClickableLabel& label) { label.setColor(Palette::Darkslategray); }
	)),
	targets.emplace_back(new ClickableLabel(L"‚¹‚Â‚ß‚¢", Window::Center().movedBy(0, labelHeight + labelInterval), font_handler, Palette::Darkslategray,
		[this](ClickableLabel&) { transition = true; },
		[](ClickableLabel& label) { label.setColor(Palette::White); },
		[](ClickableLabel& label) { label.setColor(Palette::Darkslategray); }
	));
	targets.emplace_back(new ClickableLabel(L"‚¨‚í‚é", Window::Center().movedBy(0, 2 * (labelHeight + labelInterval)), font_handler, Palette::Darkslategray,
		[this](ClickableLabel&) { System::Exit(); },
		[](ClickableLabel& label) { label.setColor(Palette::White); },
		[](ClickableLabel& label) { label.setColor(Palette::Darkslategray); }
	));


	const int panelInterval = Window::Height() / 36;
	const int panelSize = Window::Width() / 5;
	const int panelLeft = (Window::Width() - panelSize * 3 - panelInterval * 2) / 2;
	const int panelOver = (Window::Height() - panelSize) / 2;
	const int backButtonMargin = Window::Height() / 54;
	const int backButtonSize = Window::Height() / 5;
	
	targets.emplace_back(new ClickablePanel(panelSize, panelSize, Point(Window::Width() + panelLeft, panelOver), L"2PlayerPanel",
		[this](ClickablePanel&) { m_data->numOfPlayer = 2; changeScene(SceneName::Game); },
		[](ClickablePanel& panel) { panel.setTextureHandler(L"2PlayerPanel_"); },
		[](ClickablePanel& panel) { panel.setTextureHandler(L"2PlayerPanel"); }
	));
	targets.emplace_back(new ClickablePanel(panelSize, panelSize, Point(Window::Width() + panelLeft + panelSize + panelInterval, panelOver), L"3PlayerPanel",
		[this](ClickablePanel&) { m_data->numOfPlayer = 3; changeScene(SceneName::Game); },
		[](ClickablePanel& panel) { panel.setTextureHandler(L"3PlayerPanel_"); },
		[](ClickablePanel& panel) { panel.setTextureHandler(L"3PlayerPanel"); }
	));
	targets.emplace_back(new ClickablePanel(panelSize, panelSize, Point(Window::Width() + panelLeft + panelSize * 2 + panelInterval * 2, panelOver), L"4PlayerPanel",
		[this](ClickablePanel&) { m_data->numOfPlayer = 4; changeScene(SceneName::Game); },
		[](ClickablePanel& panel) { panel.setTextureHandler(L"4PlayerPanel_"); },
		[](ClickablePanel& panel) { panel.setTextureHandler(L"4PlayerPanel"); }
	));
	targets.emplace_back(new ClickablePanel(backButtonSize, backButtonSize, Point(Window::Width() + backButtonMargin, backButtonMargin), L"back",
		[this](ClickablePanel&) { transition = true; }
	));

	for (auto& target : targets) {
		clickDetector.addTarget(target);
	}
	clickDetector.addPointer(pointer);

}


void Title::update() {
	pointer->update();

	if (!transition) {
		clickDetector.update(); 
	}
	else {
		const int dir = scene == TitleScene::TOP ? -1 : 1;
		for (auto&& target : targets) {
			target->moveBy({ dir*Window::Width(), 0 });
		}
		transition = false;
		scene = (TitleScene)(((int)scene +1)%2);
	}
}

void Title::draw() const {
	//if (scene == TitleScene::TOP) TextureAsset(L"title").draw();

	for (const auto& target : targets) target->draw();

	pointer->draw();
}