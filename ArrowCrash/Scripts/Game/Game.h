#pragma once
#include "../Scene.h"
#include "GameData.h"
#include "Field.h"
#include "Player.h"


struct UIComponents {
	Line topUIBorder;
	std::array<Line, 2> playerBorder;

	void draw() const;
};


class Game : public Scene {
private:
	GameData gameData;

	std::vector<Player> players;

	UIComponents ui;

	void initGameData();
	void initUIComponents();

public:
	void init() override;

	void update() override;

	void draw() const override;
};