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

class Result {
private:
	const int numOfPlayer;
	GameData& data;
	Game& gameScene; //シーン遷移のためだけのやつ(インターフェースに分離したほうが良いかも)
	std::vector<int>& scores;
	Color backColor;
	Stopwatch timer;
	enum class State { Init, Wait, Show	} state;

	std::vector<bool> winner;
	std::vector<bool> checked;


public:
	Result(int numOfPlayer, GameData& data, Game& gameScene_, std::vector<int>& scores);
	~Result() = default;

	void update();
	void draw() const;
};

class Game : public Scene {
private:
	bool paused;
	Optional<Pause> pause; //初期化タイミングを遅らせる
	Optional<Result> result;

	bool timeUp;

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

public:
	Game();
	~Game();
	void init() override;
	void update() override;
	void draw() const override;

	void transit(SceneName nextScene) { changeScene(nextScene); }
};
