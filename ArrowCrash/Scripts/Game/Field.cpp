#include "Field.h"

Field::Field(const Point& stdPos_, std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks_)
	:Explodable(),
	stdPos(stdPos_),
	arrowBlocks(arrowBlocks_),
	fieldShape(stdPos.movedBy(Block::blockSize, 0), Size(constants::col_len - 2, constants::row_len - 1)*Block::blockSize),
	shouldCheckLine(false)
{
	for (int i = 0; i < constants::numOfItemType; i++) {
		activated.at(i) = false;
	}

	for (int i = 0; i < constants::row_len; i++) {
		blocks.emplace_back();
		for (int j = 0; j < constants::col_len; j++) {
			if (i == constants::row_len - 1 || j == 0 || j == constants::col_len - 1) {
				blocks[i].emplace_back(new InvincibleBlock({ i, j }, stdPos));
			}
			else
				blocks[i].emplace_back(nullptr);
		}
	}
}

std::vector<Field*> Field::fields;

bool Field::contains(const Point& point) const {
	return point.x >= 0 && point.x < constants::row_len &&
		point.y >= 0 && point.y < constants::col_len;
}

void Field::closeLine() {

	std::array<int, constants::row_len - 1> table; //蜷・｡後〒縺ｩ繧後□縺題ｩｰ繧√ｋ縺九ｒ險倬鹸縺吶ｋ繝・・繝悶Ν
	
	int counter = 0;

	//隧ｰ繧√ｋ陦梧焚縺ｮ險育ｮ・
	for (int i = blocks.size() - 2; i > 0 ; i--) {

		bool empty = true;
		for (int j = 1; j < constants::col_len - 1; j++) {
			if (blocks[i][j]) {
				empty = false;
				break;
			}
		}

		if (empty) {
			table[i] = 0; //遨ｺ縺ｮ陦瑚・菴薙・隧ｰ繧√↑縺上※繧医＞
			counter++;
		}

		table[i - 1] = counter;
	}

	//隧ｰ繧√ｋ
	for (int i = constants::row_len - 2; i > 0; i--) {
		if (table[i - 1] != 0) {
			for (int j = 1; j <= constants::col_len - 2; j++) {
				if (blocks[i - 1][j]) {
					auto& upper = blocks[i - 1][j];
					upper->setPoint(upper->getPoint().movedBy(1, 0));
					blocks[i][j] = upper;
					upper.reset();
				}
			}
		}
	}

	shouldCheckLine = false;
}

int Field::explode(const Point& start, ExplosionDirection direction) {

	//辷・匱譁ｹ蜷代ｒ險育ｮ・
	Point vec(0, 0);
	int tmp = (int)direction;

	//陦梧婿蜷・x譁ｹ蜷・
	if (tmp % 4 != 0) vec.y = tmp / 4 == 0 ? 1 : -1;
	//蛻玲婿蜷・y譁ｹ蜷・
	tmp = (tmp + 1) % 8;
	if (tmp % 4 != 3) vec.x = tmp / 4 == 0 ? -1 : 1;

	Point point(start);
	
	int numOfDestroyed = 0;

	do {
		if (auto& blk = blocks.at(point.x).at(point.y)) {
			if ((int)blk->ItemCheck()) {
				switch (blk->ItemCheck()) {
				case ItemType::InterruptionGuard: {
					effectOn((int)ItemType::InterruptionGuard);
					break;
				}
				case ItemType::SpeedUp:
				case ItemType::ForbidRotating: {
					for (auto field : fields) {
						if (field != this) {
							field->effectOn((int)blk->ItemCheck());
						}
					}
				}
											   break;
				default: break;
				}
				for (auto&& arr : blocks) {
					for (auto&& blk : arr) {
						if (blk)
							if ((int)blk->ItemCheck())
								blk->destroy();
					}
				}
			}
			blk->destroy();
			numOfDestroyed++;
		}
	} while (contains(point.moveBy(vec)));

	return numOfDestroyed;
}

void Field::setBlockAt(std::shared_ptr<Block> block, const Point& point) {
	blocks.at(point.x).at(point.y) = block;
}

void Field::reset() {

	//arrowBlocks縺ｮ縺・■settled縺ｪ繧ゅ・繧貞炎髯､
	auto&& itr = std::remove_if(arrowBlocks.begin(), arrowBlocks.end(), [](std::weak_ptr<ArrowBlock> blk) { return blk.lock()->isSettled(); });
	arrowBlocks.erase(itr, arrowBlocks.end());

	for (auto&& arr : blocks) {
		for (auto&& blk : arr) {
			if (blk) blk->destroy();
		}
	}

	for (auto&& arr : blocks) {
		for (auto&& block : arr) {
			if (block && block->isDestroyed()) block.reset();
		}
	}

	for (int i = 0; i < constants::numOfItemType; i++) {
		effectEnd(i);
	}
}

void Field::riseFloor(int num) {

	for (int i = 0; i < constants::row_len - 1; i++) {
		for (int j = 1; j < constants::col_len - 1; j++) {
			if (blocks[i][j]) {
				if (i < num) {
					reset();
					return;
				}

				blocks[i - num][j] = blocks[i][j];
				blocks[i - num][j]->setPoint(blocks[i][j]->getPoint().movedBy(-num, 0));
				blocks[i][j].reset();
			}
		}
	}

	for (int i = 0; i < num; i++) {
		for (int j = 1; j < constants::col_len - 1; j++) {
			Point point(constants::row_len - 2 - i, j);
			blocks[point.x][point.y].reset(new NormalBlock(point, stdPos, UnitType::I));
		}
	}
}

void Field::update() {
	for (auto&& arr : blocks) {
		for (auto&& block : arr) {
			if (block && block->isDestroyed()) block.reset();
		}
	}
	
	for (int i = 0; i < constants::numOfItemType; i++)
		switch (i) {
		case (int)ItemType::ForbidRotating: if(ItemTimers[i].s() > 5) effectEnd(i); break;
		case (int)ItemType::InterruptionGuard: if(ItemTimers[i].s() > 5) effectEnd(i); break;
		case (int)ItemType::SpeedUp: if(ItemTimers[i].s() > 10) effectEnd(i); break;
		}

	if (shouldCheckLine) closeLine();
}

void Field::draw() const {
	static const int outerFrameWidth = fieldShape.w / 50.0;
	fieldShape.drawFrame(0.0, outerFrameWidth, Palette::Darkorange)(TextureAsset(L"field_background")).draw();

	for (const auto& arr : blocks) {
		for (const auto& block : arr) {
			if (block) block->draw();
		}
	}
}

bool Field::CheckItemExistence() const{
	for (auto&& arr : blocks) {
		for (auto&& blk : arr) {
			if (blk)
				if ((int)blk->ItemCheck())
					return true;
		}
	}
	return false;
}

void Field::effectOn(int type) {
	activated[type] = true;
	ItemTimers[type].restart(); 
	PutText(L"this type is ,",type).from(stdPos + Point(64, 64));
}
void Field::effectEnd(int type) {
	activated[type] = false;
	ItemTimers[type].reset();
}