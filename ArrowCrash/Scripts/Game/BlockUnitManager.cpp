#include "BlockUnitManager.h"


BlockUnitManager::BlockUnitManager(Field& field_, std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks_, const GameData& gameData, int player_num)
	:field(field_),
	arrowBlocks(arrowBlocks_),
	stdPos(gameData.stdPositions.at(player_num)),
	blockSize(gameData.blockSize),
	hasExchanged(false),
	ojamaBuffer(0),
	nextUnitFramePos(gameData.nextUnitFramePos.at(player_num)),
	stockFramePos(gameData.stockFramePos.at(player_num)),
	currentUnit(new BlockUnit(Point(0, constants::col_len / 2 - 2), stdPos, blockSize, arrowBlocks, field)),
	stock(nullptr)
{
	for (int i = 0; i < 2; i++) {
		generate();
	}
	currentUnit->predict();
	managers.push_back(this);
}

std::vector<BlockUnitManager*> BlockUnitManager::managers;

void BlockUnitManager::generate() {
	nextUnits.emplace_back(new BlockUnit(Point(0, constants::col_len / 2 - 2), stdPos, blockSize, arrowBlocks, field));
}

void BlockUnitManager::resetField() {
	field.reset();
	currentUnit->resetPoint();
	currentUnit->restartTimer();
	currentUnit->predict();
}

void BlockUnitManager::update() {
	currentUnit->update();
	if (currentUnit->isSettled()) {

		if (ojamaBuffer > 0) field.riseFloor(ojamaBuffer);

		currentUnit = nextUnits.front();
		nextUnits.pop_front();
		generate();

		if (currentUnit->cannotSettle())
		{
			resetField();
		}

		currentUnit->predict();
		hasExchanged = false;
	}
}

void BlockUnitManager::draw() const {
	currentUnit->draw();

	int counter = 0; //vector(nextUnitsFrameInfo)—p
	for (auto&& unit : nextUnits) {
		unit->draw(nextUnitFramePos.at(counter++), 1.0);
	}
	if (stock) stock->draw(stockFramePos, 1.0);
}

void BlockUnitManager::exchangeStock() {

	if (hasExchanged) return;
	
	hasExchanged = true;

	if (stock) {
		currentUnit.swap(stock);
		currentUnit->resetPoint();

		if (currentUnit->cannotSettle())
		{
			resetField();
		}

		currentUnit->predict();
	}
	else {
		stock = std::move(currentUnit);
		currentUnit = nextUnits.front();
		nextUnits.pop_front();

		if (currentUnit->cannotSettle())
		{
			resetField();
		}

		generate();
		currentUnit->predict();
	}
}

void BlockUnitManager::bother(int numOfDestroyed) {
	int rising = numOfDestroyed / 5;
	for (auto mngr : managers) {
		if (mngr != this) {
			mngr->ojamaBuffer += rising;
		}
	}
}