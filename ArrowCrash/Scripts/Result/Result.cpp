#include "Result.h"


Result::Result() {
}

Result::~Result() {
	ymds::GamepadManager::get().inactivate();
}

void Result::init() {
	ymds::GamepadManager::get().activate();
}

void Result::update() {

	ymds::GamepadManager::get().update();

	static auto twoClicked = []() -> bool {
		return ymds::GamepadManager::get().any([](ymds::Gamepad& gamepad) { return gamepad.clicked(ymds::GamepadIn::TWO); });
	};
	if (twoClicked())
		changeScene(SceneName::Title);
}

void Result::draw() const {
	PutText(L"Result").at(Window::Center());
	for (int i = 0; i < m_data->numOfPlayer; i++) {
		PutText(ToString(i)+L":"+ToString(m_data->scores.at(i))).at(Window::Center().movedBy(0, 100 + i*50));
	}
}