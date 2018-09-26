#pragma once
#include "Field.h"
#include "BlockUnitManager.h"

class Player {
private:
	const int number; //プレイヤー番号
	const GameData& gameData;

	Field field;
	// 一括で爆発するためフィールド上の全てのArrowBlockへの参照を持っておく
	std::vector<std::weak_ptr<ArrowBlock>> arrowBlocks;
	BlockUnitManager mngr;

public:
	Player(int player_num ,const GameData& gameData);
	~Player() = default;

	void update();
	void draw() const;
};