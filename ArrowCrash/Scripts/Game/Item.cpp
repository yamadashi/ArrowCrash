#include "Item.h"

//アイテム全般
Item::Item(const Point& point_, const Point& stdPos_, const int blockSize_, Field& field_)
	:destroyed(false),
	stdPos(stdPos_),
	field(field_),
	point(point_),
	timer(true),
	blockSize(blockSize_),
	rect(stdPos.movedBy(Point(point.y, point.x)*blockSize), blockSize),
	settled(false)
{
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
					geometry[i][j] = std::make_shared<NormalBlock>(point.movedBy(i, j), stdPos, blockSize);
		}
	}
}

//着地判定
bool Item::checkCollision(const Point& point_) const {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			if (field.getAt(point_.movedBy(i, j)) && geometry[i][j])
				return true;
		}
	}
	return false;
}

//固定
void Item::settle() {

	for (auto&& arr : geometry) {
		for (auto&& block : arr) {
			if (block) {
				block->setSettled();
				field.setBlockAt(std::move(block), block->getPoint());
			}
		}
	}
	settled = true;
}

//わからん
void Item::setPoint(const Point& point_) {
	point.set(point_);
	rect.setPos(stdPos.movedBy(Point(point.y, point.x)*blockSize));
}

void Item::update() {
	if (timer.ms() > 500)
		if (checkCollision(point.movedBy(1, 0))) {
		settle();
		return;
	}
	point.moveBy(1, 0);
	timer.restart();
}

void Item::draw() const {
	
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			geometry[i][j]->draw();
		}
	}
}



//回転禁止
ForbidRotating::ForbidRotating(const Point& point_, const Point& pos, const int blockSize, Field& field_)
	:Item(point_, pos, blockSize, field_)
{}

void ForbidRotating::draw() const {
	rect.draw(Palette::Red);
}




//落下速度上昇
IncreaseFallVelocity::IncreaseFallVelocity(const Point& point_, const Point& pos, const int blockSize, Field& field_)
	:Item(point_, pos, blockSize, field_)
{}

void IncreaseFallVelocity::draw() const {
	rect.draw(Palette::Yellow);
}



//お邪魔無効
InvalidateInterruption::InvalidateInterruption(const Point& point_, const Point& pos, const int blockSize, Field& field_)
	:Item(point_, pos, blockSize, field_)
{}

void InvalidateInterruption::draw() const {
	rect.draw(Palette::Blue);
}