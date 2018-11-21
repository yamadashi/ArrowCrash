#pragma once
#include "../Scene.h"
#include "GameData.h"
#include "Field.h"
#include "Player.h"
#include "../ymdsLib/EventSystem/EventSystem.h"
#include "../ymdsLib/Clickable/ClickDetector.h"
#include "../ymdsLib/Clickable/ClickableLabel.h"
#include "../Pointer.h"

class Game;

class Pause {
private:
	const int numOfPlayer;
	ymds::ClickDetector clickDetector;
	std::vector<std::shared_ptr<ymds::Clickable>> targets;
	std::vector<std::shared_ptr<Pointer>> pointers;

public:
	Pause(int numOfPlayer, Game& gameScene);
	~Pause() = default;

	void update();
	void draw() const;

	void resetPointerPos();
};

class Game : public Scene {
private:
	bool paused;
	Optional<Pause> pause; //初期化タイミングを遅らせる

	Stopwatch timer;
	const int time_limit; //�b

	std::vector<Player> players;

	GameData gameData;

	struct UIInfo {
		int topUIHeight;
		int fieldLeftMargin;
		int fieldTopMargin;
	} uiInfo;

	std::vector<Rect> playerPanel;

	void initGameData();
	void initUIComponents();

public:
	Game();
	~Game();
	void init() override;
	void update() override;
	void draw() const override;

	void transit(SceneName nextScene) { changeScene(nextScene); }
};
