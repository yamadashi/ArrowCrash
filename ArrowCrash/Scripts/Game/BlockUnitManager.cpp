#include "BlockUnitManager.h"


BlockUnitManager::BlockUnitManager(Field& field_, std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks_, GameData& gameData, int player_num)
	:playerNum(player_num),
	field(field_),
	arrowBlocks(arrowBlocks_),
	stdPos(gameData.stdPositions.at(playerNum)),
	hasExchanged(false),
	ojamaBuffer(0),
	nextUnitFrames(gameData.nextUnitFrames.at(playerNum)),
	stockFrame(gameData.stockFrames.at(playerNum)),
	currentUnit(new BlockUnit(Point(0, constants::col_len / 2 - 2), stdPos, arrowBlocks, field)),
	stock(nullptr),
	ItemPropability(20)//パーセント表記
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
			if (field.pickUpRandomFlat() != -1) Item = std::make_shared<ItemUnit>(Point(0, field.pickUpRandomFlat()), stdPos, field);
			else Item = std::make_shared<ItemUnit>(Point(0, Random<int>(0, constants::col_len - 4)), stdPos, field);
			currentUnit = Item;
		}
		else {
			currentUnit = nextUnits.front();
			nextUnits.pop_front();
			generate();

			if (currentUnit->cannotSettle())
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

	//次ユニットとストックの表示
	{
		static const double scale = 0.60;
		const auto& frameSize = nextUnitFrames.front().size;
		const double unitScale = scale * frameSize.x / (Block::blockSize * 4);
		const Point offset = ((1.0 - scale) / 2.0 * frameSize).asPoint();

		const double nextTextureScale = 2.3 * frameSize.x / 250;
		const Point nextTexturePos = nextUnitFrames.front().pos - Point(frameSize.x * 2 / 5, frameSize.y * 3 / 4);

		TextureAsset(L"next")(250*playerNum, 0, 250, 250).scale(nextTextureScale).draw(nextTexturePos);
		int counter = 0;
		for (auto&& unit : nextUnits) {
			unit->draw(nextUnitFrames.at(counter++).pos.movedBy(offset), unitScale);
		}

		TextureAsset(L"stock")(64*playerNum, 0, 64, 64).resize(frameSize).draw(stockFrame.pos);
		if (stock) {
			stock->draw(stockFrame.pos.movedBy(offset), unitScale);
		}
	}

	if (ojamaBuffer > 0) {
		const double effectCellSize = TextureAsset(L"WARNING").width;
		TextureAsset(L"WARNING").scale(12 * Block::blockSize / effectCellSize).draw(stdPos + Point(Block::blockSize, 0.5 * Block::blockSize));
	}
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
	int rising = calculateRising(numOfDestroyed);

	if (ojamaBuffer >= rising) {
		ojamaBuffer -= rising;
		rising = 0;
		return;
	}
	else if (ojamaBuffer < rising) {
		rising -= ojamaBuffer;
		ojamaBuffer = 0;
	}
	for (auto mngr : managers) {
		if (mngr != this) {
			mngr->ojamaBuffer += rising;
		}
	}
}

int BlockUnitManager::calculateRising(int numOfDestroyed) {
	if (numOfDestroyed < 10) return 0;
	else if (numOfDestroyed < 15) return 1;
	else if (numOfDestroyed < 20) return 2;
	else if (numOfDestroyed < 30) return 3;
	else if (numOfDestroyed < 40) return 4;
	else if (numOfDestroyed < 50) return 5;
	else if (numOfDestroyed < 75) return 6;
	else if (numOfDestroyed < 100) return 7;
	else return 8;
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