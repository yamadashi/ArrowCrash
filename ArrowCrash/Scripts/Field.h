#pragma once
#include <Siv3D.hpp>
#include "Block.h"
#include "Constants.h"
#include "GameData.h"

using BlockGrid = std::vector<std::vector<std::shared_ptr<Block>>>;

class Field : public Explodable {
private:
	const Point stdPos; //äÓèÄì_
	BlockGrid cells;


public:
	Field(const Point& stdPos_, int cellSize);
	~Field() = default;

	void explode(const Point& point, ExplosionDirection direction) override;
	void setBlockAt(std::shared_ptr<Block>& block, int row, int col);

	void draw() const;
};
