#include "Block.h"


//Block

Block::Block(const Point& point_, const Point& pos, const int cellSize)
	:destroyed(false),
	point(point_),
	rect(pos, cellSize),
	settled(false)
{}



//NormalBlock

NormalBlock::NormalBlock(const Point& point_, const Point& pos, const int cellSize)
	:Block(point_, pos, cellSize)
{}

void NormalBlock::draw() const {
	rect.draw(Palette::Green);
}



//ArrowBlock

ArrowBlock::ArrowBlock(const Point& point_, const Point& pos, const int cellSize, ExplosionDirection dir, Explodable& field_)
	:Block(point_, pos, cellSize),
	direction(dir),
	field(field_)
{}

void ArrowBlock::explode() {
	field.explode(point, direction);
}

void ArrowBlock::draw() const {
	rect.draw(Palette::Red);
}



//InvincibleBlock

InvincibleBlock::InvincibleBlock(const Point& point_, const Point& pos, const int cellSize)
	:Block(point_, pos, cellSize)
{
	setSettled(); //Ç‡Ç§ÇøÇÂÇ¡Ç∆ó«Ç¢ä¥Ç∂Ç…èëÇØÇªÇ§
}

void InvincibleBlock::draw() const {
	rect.draw(Palette::Gray);
	PutText(point).from(rect.pos);
}