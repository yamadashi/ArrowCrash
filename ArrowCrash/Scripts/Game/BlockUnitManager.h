#pragma once
#include "BlockUnit.h"
#include "Item.h"
#include <queue>

class BlockUnitManager {
private:
	//“`”À‚·‚é‚½‚ß‚É•Û‚µ‚Ä‚¢‚é‚ª‚±‚ÌƒNƒ‰ƒX‚Å‚Íg‚í‚È‚¢
	Field& field;
	std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks;
	const Point stdPos;
	const int blockSize;

	std::queue<std::shared_ptr<BlockUnit>> nextUnits;
	std::shared_ptr<BlockUnit> currentBlock;
	std::shared_ptr<BlockUnit> stock;
	std::shared_ptr<Item> Item;
	void generate();

public:
	BlockUnitManager(Field& field_, std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks_, const Point& stdPos_, const int blockSize_);
	~BlockUnitManager() = default;
	void update();
	void draw() const;
	void generateItem();
	BlockUnit& getCurrentUnit() { return *currentBlock; }
};