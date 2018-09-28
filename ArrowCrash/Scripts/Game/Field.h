#pragma once
#include <Siv3D.hpp>
#include "Block.h"
#include "../Constants.h"
#include "GameData.h"

using BlockGrid = std::vector<std::vector<std::shared_ptr<Block>>>;

class Field : public Explodable {
private:
	BlockGrid blocks;
	const Point stdPos; //��_

	bool contains(const Point& point) const;

public:
	Field(const Point& stdPos_, int blockSize);
	~Field() = default;

	void explode(const Point& start, ExplosionDirection direction) override;
	void setBlockAt(std::shared_ptr<Block>& block, int row, int col);

	void update();
	void draw() const;
};
