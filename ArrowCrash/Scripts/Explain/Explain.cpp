#include "Explain.h"

Explain::Explain()
	:counter(0),
	numOfExplain(6)
{}

Explain::~Explain() {
	ymds::GamepadManager::get().inactivate();
}

void Explain::init() {

	ymds::GamepadManager::get().activate();

	//pointer
	for (int i = 0; i < 4; i++) {
		//ポインタの初期位置
		Point pos(Window::Center().movedBy(0, Window::Height() / 6));
		const Point tmp(2 * (i % 2) - 1, 2 * (i / 2) - 1); //i == 0 のとき (-1, 0), i== のとき (0, 1)
		pos.moveBy(0.2 * Window::Width() * tmp.x, 0.2 * tmp.y * Window::Height());

		pointers.emplace_back(new Pointer(i, pos));
	}

	const int buttonMargin = Window::Height() / 54;
	const int buttonSize = Window::Height() / 5;
	targets.emplace_back(new ymds::ClickablePanel(L"back", { buttonMargin, buttonMargin }, Size(buttonSize, buttonSize),
		[this](ymds::ClickablePanel&) {
		SoundAsset(L"select").playMulti();
		if (counter > 0) counter--;
		else changeScene(SceneName::Title); }
	));
	targets.emplace_back(new ymds::ClickablePanel(L"forward", { Window::Width() - buttonSize - buttonMargin, buttonMargin }, Size(buttonSize, buttonSize),
		[this](ymds::ClickablePanel&) {
		SoundAsset(L"select").playMulti();
		if (counter < numOfExplain - 1) counter++;
		else changeScene(SceneName::Title); }
	));

	for (auto& target : targets) {
		clickDetector.addTarget(target);
	}
	for (auto& pointer : pointers) {
		clickDetector.addPointer(pointer);
	}
}

void Explain::update() {

	ymds::GamepadManager::get().update();

	clickDetector.update();

	for (auto& pointer : pointers) pointer->update();
}

void Explain::draw() const {
	TextureAsset(L"background").resize(Window::Size()).draw();

	static const Rect clientRect(Window::ClientRect());
	clientRect.draw(Alpha(200));

	TextureAsset(L"explain" + ToString(counter)).resize(Window::Size()).draw();
	PutText(L"explain" + ToString(counter)).from(0 ,0);

	for (const auto& target : targets) target->draw();

	for (const auto& pointer : pointers) pointer->draw();
}