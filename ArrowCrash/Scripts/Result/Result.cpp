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
	for (int i = 0; i < m_data->numOfPlayer; i++) {
		PutText(ToString(i)+L":"+ToString(m_data->scores.at(i))).at(Window::Center().movedBy(0, 100 + i*50));
	}
}