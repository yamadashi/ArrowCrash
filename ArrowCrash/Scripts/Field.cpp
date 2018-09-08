#include "Field.h"

Field::Field(int player_num, int cellSize)
	:stdPos()
{
	for (int i = 0; i < constants::row_len; i++) {
		cells.emplace_back();
		for (int j = 0; j < constants::col_len; j++) {
			if (i == constants::row_len - 1 || j == 0 || j == constants::col_len - 1)
				cells[i].emplace_back(InvincibleBlock({ i, j }, stdPos.movedBy(i*cellSize,j*cellSize), cellSize));
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