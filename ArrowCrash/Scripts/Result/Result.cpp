#include "Result.h"


Result::Result() {

}

Result::~Result() {

}

void Result::update() {
	if (Input::KeyEnter.clicked) {
		changeScene(SceneName::Title);
	}
}

void Result::draw() const {
	PutText(L"Result").at(Window::Center());
}