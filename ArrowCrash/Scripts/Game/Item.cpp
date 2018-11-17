#include "Item.h"

ItemUnit::ItemUnit(const Point& point_, const Point& stdPos_, Field& field_)
	:Unit(point_, stdPos_, field_),
	destroyed(false),
	SpeedUpRate(33),
	ForbidRotatingRate(33),
	InterruptionGuardRate(34)
{
	auto& pattern = unitPatterns[4];

	ItemType type;
	int x = Random<int>(1, 100);
	if (0 < x && x <= SpeedUpRate) type = ItemType::SpeedUp;
	else if (SpeedUpRate < x && x <= SpeedUpRate + ForbidRotatingRate) type = ItemType::ForbidRotating;
	else if (100 - InterruptionGuardRate < x && x <= 100) type = ItemType::InterruptionGuard;

	geometry[1][1] = std::make_shared<ItemBlock>(point.movedBy(1, 1), stdPos, ItemBlock::PartPlace::UpLeft, type);
	geometry[1][2] = std::make_shared<ItemBlock>(point.movedBy(1, 2), stdPos, ItemBlock::PartPlace::UpRight, type);
	geometry[2][1] = std::make_shared<ItemBlock>(point.movedBy(2, 1), stdPos, ItemBlock::PartPlace::DownLeft, type);
	geometry[2][2] = std::make_shared<ItemBlock>(point.movedBy(2, 2), stdPos, ItemBlock::PartPlace::DownRight, type);
}

void ItemUnit::update() {
	if (timer.ms() > 75)
	{
		if (checkCollision(point.movedBy(1, 0))) {
			settle();
			return;
		}
		point.moveBy(1, 0);
		timer.restart();

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (geometry[i][j])
					geometry[i][j]->setPoint(point.movedBy(i, j));
			}
		}
	}
}

void ItemUnit::draw() const {
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (geometry[i][j]) {
				geometry[i][j]->draw();
				}
		}
	}

}