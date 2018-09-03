#pragma once
#include "Scene.h"

class Title : public Scene {
private:

public:
	void update() {
		if (Input::MouseL.clicked) changeScene(SceneName::Game);
	}

	void draw() const {
		PutText(L"Title").at(Window::Center());
	}
};