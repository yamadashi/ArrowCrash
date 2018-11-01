#include "Item.h"

ItemUnit::ItemUnit(const Point& point_, const Point& stdPos_, const int blockSize_, Field& field_)
	:Unit(point_, stdPos_, blockSize_, field_),
	destroyed(false)
{
	auto& pattern = unitPatterns[4];

	geometry[1][1] = std::make_shared<ItemBlock>(point.movedBy(1, 1), stdPos, blockSize, PartPlace::UpLeft);
	geometry[1][2] = std::make_shared<ItemBlock>(point.movedBy(1, 2), stdPos, blockSize, PartPlace::UpRight);
	geometry[2][1] = std::make_shared<ItemBlock>(point.movedBy(2, 1), stdPos, blockSize, PartPlace::DownLeft);
	geometry[2][2] = std::make_shared<ItemBlock>(point.movedBy(2, 2), stdPos, blockSize, PartPlace::DownRight);
}

void ItemUnit::update() {
	if (timer.ms() > 100)
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


/*
//âÒì]ã÷é~
ForbidRotating::ForbidRotating(const Point& point_, const Point& pos, const int blockSize, Field& field_)
	:ItemUnit(point_, pos, blockSize, field_)
{}

void ForbidRotating::draw() const {
	rect.draw(Palette::Red);
}




//óéâ∫ë¨ìxè„è∏
IncreaseFallVelocity::IncreaseFallVelocity(const Point& point_, const Point& pos, const int blockSize, Field& field_)
	:ItemUnit(point_, pos, blockSize, field_)
{}

void IncreaseFallVelocity::draw() const {
	rect.draw(Palette::Yellow);
}



//Ç®é◊ñÇñ≥å¯
InvalidateInterruption::InvalidateInterruption(const Point& point_, const Point& pos, const int blockSize, Field& field_)
	:ItemUnit(point_, pos, blockSize,field_)
{}

void InvalidateInterruption::draw() const {
	rect.draw(Palette::Blue);
}*/