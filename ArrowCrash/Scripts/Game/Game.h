#pragma once
#include "../Scene.h"
#include "GameData.h"
#include "Field.h"
#include "Player.h"
#include "../ymdsLib/EventSystem/EventSystem.h"


class Game : public Scene {
private:
	GameData gameData;
	bool pause;
	Stopwatch timer;
	const int time_limit; //•b

	std::vector<Player> players;
	struct UIInfo {
		int topUIHeight;
		Size playerRegion;
		Size fieldSize;
		int fieldLeftMargin;
		int fieldTopMargin;
		int unitFrameSize;
	} uiInfo;

	struct UIComponents {
		Line topUIBorder;
		std::vector<Line> playerBorders;
		std::vector<Rect> stockFrames;
		std::vector<std::vector<Rect>> nextUnitFrames;

		void draw() const;
	} uiComp;

	void initGameData();
	void initUIComponents();

public:
	Game();

	void init() override;

	void update() override;

	void draw() const override;
};