#include "Field.h"

Field::Field(const Point& stdPos_, std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks_)
	:Explodable(),
	stdPos(stdPos_),
	arrowBlocks(arrowBlocks_),
	fieldShape(stdPos.movedBy(Block::blockSize, 0), Size(constants::col_len - 2, constants::row_len - 1)*Block::blockSize),
	deathFlag(false)
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

bool Field::contains(const Point& point, bool ignoreEdge) const {
	if (!ignoreEdge)
		return point.x >= 0 && point.x < constants::row_len &&
			point.y >= 0 && point.y < constants::col_len;
	else
		return point.x >= 0 && point.x < constants::row_len - 1&&
			point.y >= 1 && point.y < constants::col_len - 1;
}

void Field::closeLine() {

	std::array<int, constants::row_len - 1> table; //蜷・｡後〒縺ｩ繧後□縺題ｩｰ繧√ｋ縺九ｒ險倬鹸縺吶ｋ繝・・繝悶Ν
	
	int counter = 0;

	//隧ｰ繧√ｋ陦梧焚縺ｮ險育ｮ・
	for (int i = blocks.size() - 2; i > 0 ; i--) {

		bool empty = true;
		for (int j = 1; j < constants::col_len - 1; j++) {
			if (blocks[i][j] && !blocks[i][j]->isDestroyed()) {
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
	for (int i = constants::row_len - 3; i > 0; i--) {
		if (table[i] != 0) {
			for (int j = 1; j <= constants::col_len - 2; j++) {
				if (blocks[i][j]) {
					blocks[i][j]->setPoint(blocks[i][j]->getPoint().movedBy(table[i], 0));
					blocks[i + table[i]][j] = blocks[i][j];
					blocks[i][j].reset();
				}
			}
		}
	}

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
			if (!(blk->isDestroyed()))numOfDestroyed++;
			blk->destroy();
		}
	} while (contains(point.moveBy(vec), true));

	return numOfDestroyed;
}

void Field::setBlockAt(std::shared_ptr<Block> block, const Point point) {
	blocks.at(point.x).at(point.y) = block;
}

void Field::reset() {

	SoundAsset(L"explosion").playMulti();

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

	deathFlag = true;
}

void Field::riseFloor(int num) {
	SoundAsset(L"riseFloor").play();
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
			blocks[point.x][point.y].reset(new NormalBlock(point, stdPos, UnitType::None));
		}
	}
}

void Field::update() {
	for (auto&& arr : blocks) {
		for (auto&& block : arr) {
			if (block && block->isDestroyed()) block.reset();
		}
	}
	for (int i = 0; i < constants::numOfItemType; i++) {
		int timeLimit = 0;
		switch (i) {
		case (int)ItemType::ForbidRotating: timeLimit = 5; break;
		case (int)ItemType::SpeedUp:
		case (int)ItemType::InterruptionGuard: timeLimit = 10; break;
		default: break;
		}
		if (ItemTimers[i].s() > timeLimit) effectEnd(i);
	}
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
	if (activated[type] && ItemTimers[type].s() < 2) {
		ItemTimers[type].restart(); 
		return;
	}
	activated[type] = true;
	ItemTimers[type].restart();

	String texture_name = L"";

	switch (type) {
	case (int)ItemType::ForbidRotating: texture_name += L"Forbid_effect"; break;
	case (int)ItemType::SpeedUp: texture_name += L"SpeedUp_effect"; break;
	case (int)ItemType::InterruptionGuard: texture_name += L"Guard_effect"; break;
	default: break;
	}
	const double effectCellSize = TextureAsset(texture_name).width;
	
	int OverlapAvoid = 1;
	
	for (auto timer : ItemTimers){
		if (timer.isActive() && timer.s() < 2) {
			OverlapAvoid += 2;
		}
	}
	ymds::EffectGenerator::addLinkedImage(texture_name, effectCellSize, stdPos + Point(Block::blockSize, OverlapAvoid * Block::blockSize), 12 * (double)Block::blockSize / effectCellSize, 0.01);
	SoundAsset(L"item").playMulti(0.83);
}

void Field::effectEnd(int type) {
	activated[type] = false;
	ItemTimers[type].reset();
}

int Field::pickUpRandomFlat() {
	int tmp_height = -1;
	std::vector<int> flats;

	for (int y = 1; y < constants::col_len - 1; y++) {
		for (int x = 0; x < constants::row_len; x++) {
			if (blocks[x][y]) {
				if (tmp_height == x) {
					flats.push_back(y - 2);
				}
				tmp_height = x;
				break;
			}
		}
	}

	if (flats.empty()) return -1;
	else {
		std::mt19937 get_rand_mt;
		std::shuffle(flats.begin(), flats.end(), get_rand_mt);
		return flats.front();
	}
}