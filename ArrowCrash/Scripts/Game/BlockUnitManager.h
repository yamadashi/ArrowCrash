#pragma once
#include "BlockUnit.h"
#include "Item.h"
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
	std::vector<Point> nextUnitFramePos;
	Point stockFramePos;

	std::list<std::shared_ptr<Unit>> nextUnits; //queue
	std::shared_ptr<Unit> currentUnit;
	std::shared_ptr<Unit> stock;
	
	void generate();

	std::shared_ptr<Unit> Item;

public:
	BlockUnitManager(Field& field_, std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks_, const GameData& gameData, int player_num);
	~BlockUnitManager() = default;
	void update();
	void draw() const;
	Unit& getCurrentUnit() { return *currentUnit; }
	void exchangeStock();
};