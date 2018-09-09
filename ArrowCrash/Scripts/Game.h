#pragma once
#include "Scene.h"
#include "GameData.h"
#include "Field.h"
#include "Player.h"



class Game : public Scene {
private:
	GameData data;

	std::vector<Player> players;

	void setData();

public:
	void init() override;

	void update() override;

	void draw() const override;
};