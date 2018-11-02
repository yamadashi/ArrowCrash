#include "Explain.h"

void Explain::init() {

}

void Explain::update() {
	//if (Input::KeyEnter.clicked) changeScene(SceneName::Title);
	if (ymds::GamepadManager::get().any([](ymds::Gamepad& gamepad) { return gamepad.clicked(ymds::GamepadIn::TWO); }))
		changeScene(SceneName::Title);
}

void Explain::draw() const {
	PutText(L"Explain\npress enter").at(Window::Center());
}