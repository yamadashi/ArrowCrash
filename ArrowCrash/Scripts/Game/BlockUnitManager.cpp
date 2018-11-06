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
	ItemPropability(10)//パーセント表記
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
	currentUnit->update();
	if (Item != nullptr) Item->update();
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

}

void BlockUnitManager::draw() const {
	currentUnit->draw();
    
	static const double scale = 0.75;
	const auto& frameSize = nextUnitFrames.front().size;
	const double unitScale = scale * frameSize.x / (Block::blockSize * 4);
	const Point offset = ((1.0 - scale) / 2.0 * frameSize).asPoint();

	static const Size frameTextureSize = TextureAsset(L"next").size;
	const double frameTextureScale = 2.3 * frameSize.x / frameTextureSize.x;
	const Point frameTexturePos = nextUnitFrames.front().pos - Point(frameSize.x * 2 / 5, frameSize.y * 3 / 4);

	TextureAsset(L"next").scale(frameTextureScale).draw(frameTexturePos);
	int counter = 0;
	for (auto&& unit : nextUnits) {
		unit->draw(nextUnitFrames.at(counter++).pos.movedBy(offset), unitScale);
	}
	if (stock) stock->draw(stockFrame.pos.movedBy(offset), unitScale);
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