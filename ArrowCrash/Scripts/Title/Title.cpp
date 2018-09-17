#include "Title.h"

Title::Title()
	:Scene(),
	clickDetector(),
	pointer(new Pointer(0)),
	scene(TitleScene::TOP),
	transition(false)
{
	const String font_handler = L"kokumincho30";
	const int labelInterval = 30;
	const int labelHeight = FontAsset(font_handler).height;

	targets.emplace_back(new ClickableLabel(L"ArrowCrash", Window::Center().movedBy(0, -140), font_handler));
	targets.emplace_back(new ClickableLabel(L"‚Í‚¶‚ß‚é", Window::Center(), font_handler, [this](Clickable&) { transition = true; }));
	targets.emplace_back(new ClickableLabel(L"‚¹‚Â‚ß‚¢", Window::Center().movedBy(0, labelHeight + labelInterval), font_handler));
	targets.emplace_back(new ClickableLabel(L"‚¨‚í‚é", Window::Center().movedBy(0, 2 * (labelHeight + labelInterval)), font_handler));


	const int panelInterval = 20;
	const int panelWidth = (Window::Width() - 4 * panelInterval) / 3;
	const int panelHeight = Window::Height() - 2 * panelInterval;
	
	targets.emplace_back(new ClickablePanel(panelWidth, panelHeight, Point(Window::Width() + panelInterval, panelInterval), L"2PlayerPanel", [this](Clickable&) {
		m_data->numOfPlayer = 2;
		changeScene(SceneName::Game);
	}));
	targets.emplace_back(new ClickablePanel(panelWidth, panelHeight, Point(Window::Width() + panelInterval * 2 + panelWidth, panelInterval), L"3PlayerPanel", [this](Clickable&) {
		m_data->numOfPlayer = 3;
		changeScene(SceneName::Game);
	}));
	targets.emplace_back(new ClickablePanel(panelWidth, panelHeight, Point(Window::Width() + panelInterval * 3 + 2 * panelWidth, panelInterval), L"4PlayerPanel", [this](Clickable&) {
		m_data->numOfPlayer = 4;
		changeScene(SceneName::Game);
	}));

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