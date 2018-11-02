#pragma once
#include "BlockUnit.h"
#include <queue>

class BlockUnitManager {
private:
	Field& field;
	std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks;
	const Point stdPos;
	bool hasExchanged; //ストック交換フラグ
	int ojamaBuffer;


	//描画用
	std::vector<Point> nextUnitFramePos;
	Point stockFramePos;

	std::list<std::shared_ptr<BlockUnit>> nextUnits; //queue
	std::shared_ptr<BlockUnit> currentUnit;
	std::shared_ptr<BlockUnit> stock;
	
	void generate();
	void resetField();

public:
	BlockUnitManager(Field& field_, std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks_, const GameData& gameData, int player_num);
	~BlockUnitManager() = default;
	void init() { managers.emplace_back(this); }
	void update();
	void draw() const;
	BlockUnit& getCurrentUnit() { return *currentUnit; }
	void exchangeStock();
	void bother(int numOfDestroyed);

	static std::vector<BlockUnitManager*> managers;
	static void clearManagerPtr() { managers.clear(); }
};