#pragma once
#include <Siv3D.hpp>
#include "Block.h"
#include "Scene.h"
#include "Constants.h"


class Field : public Explodable {
private:
	Point stdPos; //äÓèÄì_
	BlockGrid cells;

public:
	Field(int player_num, int cellSize);
	virtual ~Field() = default;

	void explode(const Point& point, ExplosionDirection direction) override;
	void setBlockAt(std::shared_ptr<Block>& block, int row, int col);

	void draw() const;
};
