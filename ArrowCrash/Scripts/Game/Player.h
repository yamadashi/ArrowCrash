#pragma once
#include "BlockUnitManager.h"
#include "OjamaGauge.h"
#include "../ymdsLib/GamePad/GamepadManager.h"

class Player {
private:
	const int number; //プレイヤー番号
	GameData& gameData;
	int score;
	int timer;

	// 一括で爆発するためフィールド上の全てのArrowBlockへの参照を持っておく
	std::shared_ptr<std::vector<std::weak_ptr<ArrowBlock>>> arrowBlocks;
	std::shared_ptr<Field> field;
	std::shared_ptr<BlockUnitManager> mngr;
	Optional<OjamaGauge> ojamaGauge;

public:
	Player(int player_num, GameData& gameData);
	~Player() = default;


	void init() { mngr->init(); field->init(); }
	void update();
	void draw() const;

	void explode();
	int getScore() const { return score; }
};