#include "BlockUnit.h"


Unit::Unit(const Point& point_, const Point& stdPos_, Field& field_)
	:field(field_),
	point(point_),
	predictedPoint(point_),
	settled(false),
	timer(true),
	stdPos(stdPos_),
	normalSpeed(1500),
	highSpeed(280),
	speed(1500),
	cannotRotate(false)
{}

bool Unit::checkCollision(const Point& point_) const {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			auto& tmp = field.getAt(point_.movedBy(i, j));
			if (tmp && !tmp->isDestroyed() && geometry[i][j])
				return true;
		}
	}
	return false;
}

void Unit::settle() {

	for (auto&& arr : geometry) {
		for (auto&& block : arr) {
			if (block) {
				block->setSettled();
				field.setBlockAt(std::move(block), block->getPoint());
			}
		}
	}
	settled = true;
}

int Unit::countNumOfBlock(const bool pattern[4][4]) const {
	int count = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (pattern[i][j]) count++;
		}
	}
	return count;
}

void Unit::resetPoint() {
	point.set(0, constants::col_len / 2 - 2);
}

bool Unit::cannotSettle() {
	Point point_(point.movedBy(-4, 0));
	while (!checkCollision(point_)) {
		point_.moveBy(1, 0);
	}
	point_.moveBy(-1, 0); //ここダサい

	if (point_.x < 0) return true;
	return false;
}

void Unit::fallImmediately() {
	point.set(predictedPoint);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (geometry[i][j])
				geometry[i][j]->setPoint(point.movedBy(i, j));
		}
	}
	settle();
}

void Unit::move(MoveDirection mov) {

	if (mov == MoveDirection::Down) {
		if (checkCollision(point.movedBy(1, 0))) {
			settle();
			return;
		}
		point.moveBy(1, 0);
		timer.restart();
	}
	else {
		int direction = mov == MoveDirection::Left ? -1 : 1;
		if (checkCollision(point.movedBy(0, direction))) return;
		point.moveBy(0, direction);
		predict();
	}


	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (geometry[i][j])
				geometry[i][j]->setPoint(point.movedBy(i, j));
		}
	}

}

void Unit::rotate(RotateDirection rot) {
	if (cannotRotate) {
		const int EffectSize = TextureAsset(L"cannotRotate").height;
		ymds::EffectGenerator::addLinkedImage(L"cannotRotate", EffectSize, stdPos.movedBy(getUnitCenter()), (double)Block::blockSize * 2 / EffectSize, 0.2);
		return;
	}

	auto prev = geometry;

	if (rot == RotateDirection::Right) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				geometry[j][3 - i] = prev[i][j];
			}
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				geometry[3 - j][i] = prev[i][j];
			}
		}
	}

	if (checkCollision(point)) {
		geometry = prev;
		return;
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (geometry[i][j]) {
				geometry[i][j]->setPoint(point.movedBy(i, j));
				geometry[i][j]->rotate(rot);
			}
		}
	}
	predict();
}

void Unit::predict() {
	predictedPoint.set(point);
	while (!checkCollision(predictedPoint)) {
		predictedPoint.moveBy(1, 0);
	}
	predictedPoint.moveBy(-1, 0); //ここダサい
}

void Unit::changeSpeed(bool act) {
	if (act)
		speed = highSpeed;
	else 
		speed = normalSpeed;
}

void Unit::changeForbid(bool act) {
	cannotRotate = act;
}

Point Unit::getUnitCenter() {
	Vec2 center = { 0, 0 };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (geometry[i][j]) {
				center += geometry[i][j]->getPoint();
			}
		}
	}
	return { int(center.y / 4 * Block::blockSize) , int(center.x / 4 * Block::blockSize) };
}


BlockUnit::BlockUnit(const Point& point_, const Point& stdPos_, std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks_, Field& field_)
	:Unit( point_, stdPos_, field_),
	arrowProbability(0.80),
	arrowBlocks(arrowBlocks_),
	type((UnitType)Random<int>(0, 6))
{
	auto& pattern = unitPatterns[(int)type];
	
	//ArrowBlock生成(分かり辛くてごめんやで)
	int arrowOrder;
	arrowOrder = Random() < arrowProbability ? Random<int>(0, countNumOfBlock(pattern) - 1) : -1; //-1は無効値


	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (pattern[i][j]) {
				if (arrowOrder-- == 0) {
					auto ptr = std::make_shared<ArrowBlock>(point.movedBy(i, j), stdPos, (ExplosionDirection)Random<int>(0, 7), field);
					geometry[i][j] = ptr;
					arrowBlocks.emplace_back(ptr);
				}
				else
					geometry[i][j] = std::make_shared<NormalBlock>(point.movedBy(i, j), stdPos, type);
			}
			else {
				geometry[i][j] = nullptr;
			}
		}
	}
	predict();
	//フィールド上部まで積まれている
	if (predictedPoint.x < 0) {
		field.reset();
		resetPoint();
		timer.restart();
		predict();
	}
}

void BlockUnit::update() {
	if (timer.ms() > speed)
	{
		if (checkCollision(point.movedBy(1, 0))) {
			settle();
			return;
		}
		point.moveBy(1, 0);
		timer.restart();

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (geometry[i][j])
					geometry[i][j]->setPoint(point.movedBy(i, j));
			}
		}
	}
}

void BlockUnit::draw() const {

	const Point predictedPos(stdPos.movedBy(predictedPoint.y * Block::blockSize, predictedPoint.x * Block::blockSize));

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (geometry[i][j]) {
				geometry[i][j]->draw();
				Rect(predictedPos.movedBy(j*Block::blockSize, i*Block::blockSize), Block::blockSize).drawFrame(Block::blockSize / 10, 0.0);
			}
		}
	}

}

void BlockUnit::draw(const Point& pos, double scale) const {

	const int scaledSize = Block::blockSize * scale;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (geometry[i][j]) {
				geometry[i][j]->draw(pos.movedBy(scaledSize*Point(j, i)), scale);
			}
		}
	}
}
