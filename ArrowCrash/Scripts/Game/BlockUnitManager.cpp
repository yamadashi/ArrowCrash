#include "BlockUnitManager.h"


BlockUnitManager::BlockUnitManager(Field& field_, std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks_, GameData& gameData, int player_num)
	:field(field_),
	arrowBlocks(arrowBlocks_),
	stdPos(gameData.stdPositions.at(player_num)),
	hasExchanged(false),
	ojamaBuffer(0),
	nextUnitFrames(gameData.nextUnitFrames->at(player_num)),
	stockFrame(gameData.stockFrames->at(player_num)),
	currentUnit(new BlockUnit(Point(0, constants::col_len / 2 - 2), stdPos, arrowBlocks, field)),
	stock(nullptr),
	ItemPropability(50)//パーセント表記
{
	for (int i = 0; i < 2; i++) {
		generate();
	}
	currentUnit->predict();
}

std::vector<BlockUnitManager*> BlockUnitManager::managers;

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

	for (int i = 0; i < constants::numOfItemType; i++) {
		switch (i) {
		case (int)ItemType::ForbidRotating: changeForbid(field.getActivatedEffect()[i]); break;
		case (int)ItemType::SpeedUp: changeSpeed(field.getActivatedEffect()[i]); break;
		case (int)ItemType::InterruptionGuard:
			if (field.getActivatedEffect()[i]) GuardOn(); break;
		}
	}

	currentUnit->update();
	if (currentUnit->isSettled()) {

		if (ojamaBuffer > 0) {
			field.riseFloor(ojamaBuffer);
			ojamaBuffer = 0;
		}

		if (!field.CheckItemExistence() && ItemPropability >= Random<int>(1, 100)) {
			Item = std::shared_ptr<Unit>(new ItemUnit(Point(0, Random<int>(0, constants::col_len - 4)), stdPos, field));
			currentUnit = Item;
		}
		else {
			currentUnit = nextUnits.front();
			nextUnits.pop_front();
			generate();

			if (currentUnit->cannotSettle()) //気持ち悪い文法...
			{
				resetField();
			}

			currentUnit->predict();
		}
		hasExchanged = false;
	}

	PutText(L"ojama:", ojamaBuffer).from(stdPos);
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
	if (currentUnit->ItemCheck())return;
	
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

void BlockUnitManager::changeForbid(bool active){
	currentUnit->changeForbid(active);
}

void BlockUnitManager::changeSpeed(bool active) {
	currentUnit->changeSpeed(active);
}

void BlockUnitManager::GuardOn() {
	ojamaBuffer = 0;
}