#include "BlockUnitManager.h"


BlockUnitManager::BlockUnitManager(Field& field_, std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks_, const GameData& gameData, int player_num)
	:field(field_),
	arrowBlocks(arrowBlocks_),
	stdPos(gameData.stdPositions.at(player_num)),
	hasExchanged(false),
	nextUnitFramePos(gameData.nextUnitFramePos.at(player_num)),
	stockFramePos(gameData.stockFramePos.at(player_num)),
	currentUnit(new BlockUnit(Point(0, constants::col_len / 2 - 2), stdPos, arrowBlocks, field)),
	stock(nullptr),
	ItemPropability(10)//パーセント表記
{
	for (int i = 0; i < 2; i++) {
		generate();
	}
	currentUnit->predict();
}

void BlockUnitManager::generate() {
	nextUnits.emplace_back(new BlockUnit(Point(0, constants::col_len / 2 - 2), stdPos, arrowBlocks, field));
}

void BlockUnitManager::resetField() {
	field.reset();
	currentUnit->resetPoint();
	currentUnit->restartTimer();
	currentUnit->predict();
}

void BlockUnitManager::update() {
	currentUnit->update();
	if (Item != nullptr) Item->update();
	if (currentUnit->isSettled()) {

		if (!field.CheckItemExistence() && ItemPropability >= Random<int>(1, 100)) {
			Item = std::shared_ptr<Unit>(new ItemUnit(Point(0, Random<int>(0, constants::col_len - 4)), stdPos, field));
			currentUnit = Item;
		}
		else {
			currentUnit = nextUnits.front();
			nextUnits.pop_front();
			generate();

			if (currentUnit->checkStackedFully()) //気持ち悪い文法...
			{
				resetField();
			}

			currentUnit->predict();
		}
		hasExchanged = false;
	}
}

void BlockUnitManager::draw() const {
	currentUnit->draw();
	if (Item != nullptr) Item->draw();
	int counter = 0; //vector(nextUnitsFrameInfo)用
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