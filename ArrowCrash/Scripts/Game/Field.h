#pragma once
#include <Siv3D.hpp>
#include "Block.h"
#include "../Constants.h"
#include "GameData.h"



using BlockGrid = std::vector<std::vector<std::shared_ptr<Block>>>;

class Field : public Explodable {
private:
	BlockGrid blocks;
	const Point stdPos; //Šî€“_
	std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks;

	bool contains(const Point& point) const;

public:
	Field(const Point& stdPos_, int blockSize, std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks);
	~Field() = default;

	void explode(const Point& start, ExplosionDirection direction) override;
	void setBlockAt(std::shared_ptr<Block> block, const Point& point);
	std::shared_ptr<Block> getAt(const Point& point) const {
		return contains(point) ?
			blocks[point.x][point.y] : nullptr;
	}
	void reset();

	void update();
	void draw() const;
};
