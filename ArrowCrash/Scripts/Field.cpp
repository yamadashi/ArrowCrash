#include "Field.h"

Field::Field(const Point& stdPos_, int cellSize)
{
	//stdPos.set();

	for (int i = 0; i < constants::row_len; i++) {
		cells.emplace_back();
		for (int j = 0; j < constants::col_len; j++) {
			if (i == constants::row_len - 1 || j == 0 || j == constants::col_len - 1) {
				Point point(i, j);
				cells[i].emplace_back(InvincibleBlock(point, stdPos.movedBy(point*cellSize), cellSize));
			}
			else
				cells[i].emplace_back(nullptr);
		}
	}



}

void Field::explode(const Point& point, ExplosionDirection direction) {

}

void Field::setBlockAt(std::shared_ptr<Block>& block, int row, int col) {

}

void Field::draw() const {

}