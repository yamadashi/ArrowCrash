#include "Field.h"

Field::Field(const Point& stdPos_, int blockSize)
	:Explodable(),
	stdPos(stdPos_)
{
	for (int i = 0; i < constants::row_len; i++) {
		blocks.emplace_back();
		for (int j = 0; j < constants::col_len; j++) {
			if (i == constants::row_len - 1 || j == 0 || j == constants::col_len - 1) {
				blocks[i].emplace_back(new InvincibleBlock(
					{ i, j }, stdPos.movedBy(Point(j, i)*blockSize), blockSize
				));
			}
			else
				blocks[i].emplace_back(nullptr);
		}
	}

}

void Field::explode(const Point& point, ExplosionDirection direction) {
	//point‚©‚çdirection•ûŒü‚É”š”­‚³‚¹‚é
}

void Field::setBlockAt(std::shared_ptr<Block>& block, int row, int col) {
	blocks.at(row).at(col) = block;
}

void Field::draw() const {
	for (const auto& arr : blocks) {
		for (const auto& block : arr) {
			if (block != nullptr) block->draw();
		}
	}
}