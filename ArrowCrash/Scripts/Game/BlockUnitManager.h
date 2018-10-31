#pragma once
#include "BlockUnit.h"
#include <queue>

class BlockUnitManager {
private:
	//伝搬するために保持しているがこのクラスでは使わない
	Field& field;
	std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks;
	const Point stdPos;
	const int blockSize;
	bool hasExchanged; //ストック交換フラグ

	//描画用
	std::vector<Rect>& nextUnitFrames;
	Rect& stockFrame;

	std::list<std::shared_ptr<BlockUnit>> nextUnits; //queue
	std::shared_ptr<BlockUnit> currentUnit;
	std::shared_ptr<BlockUnit> stock;
	
	void generate();
	void resetField();

public:
	BlockUnitManager(Field& field_, std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks_, GameData& gameData, int player_num);
	~BlockUnitManager() = default;
	void update();
	void draw() const;
	BlockUnit& getCurrentUnit() { return *currentUnit; }
	void exchangeStock();
};