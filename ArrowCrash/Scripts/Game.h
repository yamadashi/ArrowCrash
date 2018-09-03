#pragma once
#include "Scene.h"


class Game : public Scene {
private:

public :
	void update() {
		if (Input::MouseL.clicked) changeScene(SceneName::Title);
	}

	void draw() const {
		PutText(L"Game").at(Window::Center());
	}
};