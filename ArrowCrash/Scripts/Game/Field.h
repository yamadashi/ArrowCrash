#pragma once
#include <Siv3D.hpp>
#include "Block.h"
#include "../Constants.h"
#include "GameData.h"



using BlockGrid = std::vector<std::vector<std::shared_ptr<Block>>>;

class Field : public Explodable {
private:
	BlockGrid blocks;
	const Point stdPos; //基準点
	std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks;

	const Point backgroundPos;
	const Size backgroundSize;

	bool contains(const Point& point) const;
	void closeLine(); //行詰め

public:
	Field(const Point& stdPos_, std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks);
	~Field() = default;

	int explode(const Point& start, ExplosionDirection direction) override; //破壊したブロックの数を返す
	void setBlockAt(std::shared_ptr<Block> block, const Point& point);
	std::shared_ptr<Block> getAt(const Point& point) const {
		return contains(point) ?
			blocks[point.x][point.y] : nullptr;
	}
	void reset();
	void riseFloor(int rising);

	void update();
	void draw() const;

	bool shouldCheckLine; //行詰めをチェックするかどうか
	bool CheckItemExistence() const;
};
