#include "BlockUnitManager.h"


BlockUnitManager::BlockUnitManager(Field& field_, std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks_, GameData& gameData, int player_num)
	:field(field_),
	arrowBlocks(arrowBlocks_),
	stdPos(gameData.stdPositions.at(player_num)),
	blockSize(gameData.blockSize),
	hasExchanged(false),
	nextUnitFrames(gameData.nextUnitFrames->at(player_num)),
	stockFrame(gameData.stockFrames->at(player_num)),
	currentUnit(new BlockUnit(Point(0, constants::col_len / 2 - 2), stdPos, blockSize, arrowBlocks, field)),
	stock(nullptr)
{
	for (int i = 0; i < 2; i++) {
		generate();
	}
	currentUnit->predict();
}

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
		currentUnit = nextUnits.front();
		nextUnits.pop_front();
		generate();

		if (currentUnit->checkStackedFully()) //気持ち悪い文法...
		{
			resetField();
		}

		currentUnit->predict();
		hasExchanged = false;
	}
}

void BlockUnitManager::draw() const {
	currentUnit->draw();

	static const double scale = 0.75;
	static const Point offset = ((1.0 - scale) / 2.0 * nextUnitFrames.front().size).asPoint();
	int counter = 0; //vector(nextUnitsFrameInfo)用
	for (auto&& unit : nextUnits) {
		unit->draw(nextUnitFrames.at(counter++).pos.movedBy(offset), scale);
	}
	if (stock) stock->draw(stockFrame.pos.movedBy(offset), scale);
}

void BlockUnitManager::exchangeStock() {

	if (hasExchanged) return;
	
	hasExchanged = true;

	if (stock) {
		currentUnit.swap(stock);
		currentUnit->resetPoint();

		if (currentUnit->checkStackedFully()) //気持ち悪い文法...
		{
			resetField();
		}

		currentUnit->predict();
	}
	else {
		stock = std::move(currentUnit);
		currentUnit = nextUnits.front();
		nextUnits.pop_front();

		if (currentUnit->checkStackedFully()) //気持ち悪い文法...
		{
			resetField();
		}

		generate();
		currentUnit->predict();
	}
}