#include "Field.h"

Field::Field(const Point& stdPos_, int blockSize, std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks_)
	:Explodable(),
	stdPos(stdPos_),
	arrowBlocks(arrowBlocks_)
{
	for (int i = 0; i < constants::row_len; i++) {
		blocks.emplace_back();
		for (int j = 0; j < constants::col_len; j++) {
			if (i == constants::row_len - 1 || j == 0 || j == constants::col_len - 1) {
				blocks[i].emplace_back(new InvincibleBlock(
					{ i, j }, stdPos, blockSize
				));
			}
			else
				blocks[i].emplace_back(nullptr);
		}
	}
}

bool Field::contains(const Point& point) const {
	return point.x >= 0 && point.x < constants::row_len &&
		point.y >= 0 && point.y < constants::col_len;
}

void Field::explode(const Point& start, ExplosionDirection direction) {

	//爆発方向を計算
	Point vec(0, 0);
	int tmp = (int)direction;

	//行方向(x方向)
	if (tmp % 4 != 0) vec.y = tmp / 4 == 0 ? 1 : -1;
	//列方向(y方向)
	tmp = (tmp + 1) % 8;
	if (tmp % 4 != 3) vec.x = tmp / 4 == 0 ? -1 : 1;

	Point point(start);
	
	do {
		if (auto&& blk = blocks.at(point.x).at(point.y)) {
			blk->destroy();
		}
	} while (contains(point.moveBy(vec)));
}

void Field::setBlockAt(std::shared_ptr<Block> block, const Point& point) {
	blocks.at(point.x).at(point.y) = block;
}

void Field::reset() {

	//arrowBlocksのうちsettledなものを削除
	auto&& itr = std::remove_if(arrowBlocks.begin(), arrowBlocks.end(), [](std::weak_ptr<ArrowBlock> blk) { return blk.lock()->isSettled(); });
	arrowBlocks.erase(itr, arrowBlocks.end());

	for (auto&& arr : blocks) {
		for (auto&& blk : arr) {
			if (blk) blk->destroy();
		}
	}
	//ダサいからなんとかしたい
	for (auto&& arr : blocks) {
		for (auto&& block : arr) {
			if (block && block->isDestroyed()) block.reset();
		}
	}
}

void Field::update() {
	for (auto&& arr : blocks) {
		for (auto&& block : arr) {
			if (block && block->isDestroyed()) block.reset();
		}
	}
}

void Field::draw() const {
	for (const auto& arr : blocks) {
		for (const auto& block : arr) {
			if (block) block->draw();
		}
	}
}
