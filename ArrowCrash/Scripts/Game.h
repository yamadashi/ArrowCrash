#pragma once
#include "Scene.h"
#include "GameData.h"
#include "Constants.h"


class Game : public Scene {
private:
	GameData data;
	void setData();

public:
	void init() override {
		setData();
	}

	void update() override {
		if (Input::MouseL.clicked) changeScene(SceneName::Title);
	}

	void draw() const override {
		PutText(L"Game").at(Window::Center());
	}
};