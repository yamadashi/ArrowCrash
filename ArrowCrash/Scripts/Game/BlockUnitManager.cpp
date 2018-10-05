#include "BlockUnitManager.h"


BlockUnitManager::BlockUnitManager(Field& field_, std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks_, const Point& stdPos_, const int blockSize_)
	:field(field_),
	arrowBlocks(arrowBlocks_),
	stdPos(stdPos_),
	blockSize(blockSize_),
	currentBlock(new BlockUnit(Point(0, constants::col_len / 2 - 2), stdPos, blockSize, arrowBlocks, field)),
	stock(nullptr)
{
	for (int i = 0; i < 2; i++) {
		generate();
	}
}

void BlockUnitManager::generate() {
	nextUnits.emplace(new BlockUnit(Point(0, constants::col_len / 2 - 2), stdPos, blockSize, arrowBlocks, field));
}

void BlockUnitManager::update() {
	currentBlock->update();
	if (currentBlock->isSettled()) {
		currentBlock = nextUnits.front();
		nextUnits.pop();
		generate();
		currentBlock->predict();
	}
}

void BlockUnitManager::draw() const {
	currentBlock->draw();
	PutText(nextUnits.size()).from(stdPos);
}