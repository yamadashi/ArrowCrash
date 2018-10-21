#include "Block.h"


//Block

Block::Block(const Point& point_, const Point& stdPos_, const int blockSize_)
	:destroyed(false),
	stdPos(stdPos_),
	point(point_),
	blockSize(blockSize_),
	rect(stdPos.movedBy(Point(point.y, point.x)*blockSize), blockSize),
	settled(false)
{
}

void Block::setPoint(const Point& point_) {
	point.set(point_);
	rect.setPos(stdPos.movedBy(Point(point.y, point.x)*blockSize));
}

void Block::destroy() {
	ymds::EffectGenerator::addLinkedImage(L"explosion", 500, rect.pos, double(blockSize)/500, 0.1);
	destroyed = true;
}


//NormalBlock

NormalBlock::NormalBlock(const Point& point_, const Point& pos, const int blockSize)
	:Block(point_, pos, blockSize)
{}

void NormalBlock::draw() const {
	rect.draw(Palette::Green);
}



//ArrowBlock

ArrowBlock::ArrowBlock(const Point& point_, const Point& pos, const int blockSize, ExplosionDirection dir, Explodable& field_)
	:Block(point_, pos, blockSize),
	direction(dir),
	field(field_)
{}

void ArrowBlock::explode() {
	field.explode(point, direction);
}

void ArrowBlock::rotate(RotateDirection rot) {
	//矢印の向きを変更
	direction = ExplosionDirection((8 + ((int)direction + (rot == RotateDirection::Right ? 2 : -2))) % 8);
}

void ArrowBlock::draw() const {
	rect(TextureAsset(L"arrow")(50 * (int)direction, 0, 50, 50)).draw();
}



//InvincibleBlock

InvincibleBlock::InvincibleBlock(const Point& point_, const Point& pos, const int blockSize)
	:Block(point_, pos, blockSize)
{
	setSettled(); //もうちょっと良い感じに書けそう
}

void InvincibleBlock::draw() const {
	rect.draw(Palette::Gray);
}