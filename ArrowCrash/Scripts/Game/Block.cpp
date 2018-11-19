#include "Block.h"


//Block

Block::Block(const Point& point_, const Point& stdPos_)
	:destroyed(false),
	stdPos(stdPos_),
	point(point_),
	rect(stdPos.movedBy(Point(point.y, point.x)*blockSize), blockSize),
	settled(false)
{
}

int Block::blockSize = 0;

void Block::setPoint(const Point& point_) {
	point.set(point_);
	rect.setPos(stdPos.movedBy(Point(point.y, point.x)*blockSize));
}

void Block::destroy() {
	const int explosionCellSize = TextureAsset(L"explosion").height;
	ymds::EffectGenerator::addLinkedImage(L"explosion", explosionCellSize, rect.pos, double(blockSize)/explosionCellSize, 0.2);
	destroyed = true;
}


//NormalBlock

NormalBlock::NormalBlock(const Point& point_, const Point& pos, UnitType type_)
	:Block(point_, pos),
	type(type_)
{}

void NormalBlock::draw() const {
	rect(TextureAsset(L"block")(64 * (int)type, 0, 64, 64)).draw();
}

void NormalBlock::draw(const Point& pos, double scale) const {
	rect.scaled(scale).setPos(pos)(TextureAsset(L"block")(64 * (int)type, 0, 64, 64)).draw();
}


//ArrowBlock

ArrowBlock::ArrowBlock(const Point& point_, const Point& pos, ExplosionDirection dir, Explodable& field_)
	:Block(point_, pos),
	direction(dir),
	field(field_)
{}

int ArrowBlock::explode() {
	//destroy();
	//return field.explode(point, direction) + 1; //é©ï™ÇÃï™Çë´ÇµÇƒÇ¢ÇÈ
	return field.explode(point, direction);
}

void ArrowBlock::rotate(RotateDirection rot) {
	//ñÓàÛÇÃå¸Ç´ÇïœçX
	direction = ExplosionDirection((8 + ((int)direction + (rot == RotateDirection::Right ? 2 : -2))) % 8);
}

void ArrowBlock::draw() const {
	rect(TextureAsset(L"arrow")(64 * (int)direction, 0, 64, 64)).draw();
}

void ArrowBlock::draw(const Point& pos, double scale) const {
	rect.scaled(scale).setPos(pos)(TextureAsset(L"arrow")(64 * (int)direction, 0, 64, 64)).draw();
}



//ItemBlock

ItemBlock::ItemBlock(const Point& point_, const Point& stdPos, const PartPlace Part_, const ItemType Type_)
	:Block(point_, stdPos),
	Part(Part_),
	Type(Type_)
{}

void ItemBlock::draw() const {

	switch (Type) {
	case ItemType::ForbidRotating: rect(TextureAsset(L"Forbid")(32 * (int)Part, 0, 32, 32)).draw(); break;
	case ItemType::SpeedUp: rect(TextureAsset(L"SpeedUp")(32 * (int)Part, 0, 32, 32)).draw(); break;
	case ItemType::InterruptionGuard: rect(TextureAsset(L"InterruptionGuard")(32 * (int)Part, 0, 32, 32)).draw(); break;
	default: break;
	}

}

void ItemBlock::draw(const Point& pos, double scale) const {
	rect.scaled(scale).setPos(pos)(TextureAsset(L"block")).draw();
}


//InvincibleBlock

InvincibleBlock::InvincibleBlock(const Point& point_, const Point& pos)
	:Block(point_, pos)
{
	setSettled(); //Ç‡Ç§ÇøÇÂÇ¡Ç∆ó«Ç¢ä¥Ç∂Ç…èëÇØÇªÇ§
}

void InvincibleBlock::draw() const {}