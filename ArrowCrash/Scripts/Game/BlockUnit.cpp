#include "BlockUnit.h"

Unit::Unit(const Point& point_, const Point& stdPos_, const int blockSize_, Field& field_)
	:field(field_),
	point(point_),
	predictedPoint(point_),
	settled(false),
	timer(true),
	blockSize(blockSize_),
	stdPos(stdPos_)
{}

bool Unit::checkCollision(const Point& point_) const {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (field.getAt(point_.movedBy(i, j)) && geometry[i][j])
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


void Unit::update() {
	if (timer.ms() > 1500)
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

void Unit::draw() const {
	
	const Point predictedPos(stdPos.movedBy(predictedPoint.y * blockSize, predictedPoint.x * blockSize));

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (geometry[i][j]) {
				geometry[i][j]->draw();
				Rect(predictedPos.movedBy(j*blockSize, i*blockSize), blockSize).drawFrame(blockSize / 10, 0.0);
			}
		}
	}

}

void Unit::draw(const Point& pos, double scale) const {
	
	const int scaledSize = blockSize * scale;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (geometry[i][j]) {
				geometry[i][j]->draw(pos.movedBy(scaledSize*Point(j,i)), scale);
			}
		}
	}
}

void Unit::resetPoint() {
	point.set(0, constants::col_len / 2 - 2);
}


BlockUnit::BlockUnit(const Point& point_, const Point& stdPos_, const int blockSize_, std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks_, Field& field_)
	:Unit( point_, stdPos_, blockSize_, field_),
	arrowProbability(0.80),
	arrowBlocks(arrowBlocks_)
{
	auto& pattern = unitPatterns[Random<int>(0, 6)];
	
	//ArrowBlock生成(分かり辛くてごめんやで)
	int arrowOrder;
	arrowOrder = Random() < arrowProbability ? Random<int>(0, countNumOfBlock(pattern) - 1) : -1; //-1は無効値


	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (pattern[i][j]) {
				if (arrowOrder-- == 0) {
					auto ptr = std::make_shared<ArrowBlock>(point.movedBy(i, j), stdPos, blockSize, (ExplosionDirection)Random<int>(0, 7), field);
					geometry[i][j] = ptr;
					arrowBlocks.emplace_back(ptr);
				}
				else
					geometry[i][j] = std::make_shared<NormalBlock>(point.movedBy(i, j), stdPos, blockSize);
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

void BlockUnit::fallImmediately() {
	point.set(predictedPoint);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (geometry[i][j])
				geometry[i][j]->setPoint(point.movedBy(i, j));
		}
	}
	settle();
}

void BlockUnit::move(MoveDirection mov) {

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

void BlockUnit::rotate(RotateDirection rot) {

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

void BlockUnit::predict() {
	predictedPoint.set(point.movedBy(-4, 0));
	while (!checkCollision(predictedPoint)) {
		predictedPoint.moveBy(1, 0);
	}
	predictedPoint.moveBy(-1, 0); //ここダサい
}

/*
BlockUnit::BlockUnit(const Point& point_, const Point& stdPos_, const int blockSize_, std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks_, Field& field_)
	:field(field_),
	point(point_),
	predictedPoint(point_),
	settled(false),
	timer(true),
	blockSize(blockSize_),
	stdPos(stdPos_),
	arrowProbability(0.80),
	arrowBlocks(arrowBlocks_)
{
	auto& pattern = unitPatterns[Random<int>(0, 6)];
	
	//ArrowBlock生成(分かり辛くてごめんやで)
	int arrowOrder;
	arrowOrder = Random() < arrowProbability ? Random<int>(0, countNumOfBlock(pattern) - 1) : -1; //-1は無効値

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (pattern[i][j]) {
				if (arrowOrder-- == 0) {
					auto ptr = std::make_shared<ArrowBlock>(point.movedBy(i, j), stdPos, blockSize, (ExplosionDirection)Random<int>(0, 7), field);
					geometry[i][j] = ptr;
					arrowBlocks.emplace_back(ptr);
				}
				else
					geometry[i][j] = std::make_shared<NormalBlock>(point.movedBy(i, j), stdPos, blockSize);
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



bool BlockUnit::checkCollision(const Point& point_) const {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (field.getAt(point_.movedBy(i, j)) && geometry[i][j])
				return true;
		}
	}
	return false;
}

void BlockUnit::settle() {

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

int BlockUnit::countNumOfBlock(const bool pattern[4][4]) const {
	int count = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (pattern[i][j]) count++;
		}
	}
	return count;
}


void BlockUnit::update() {

	if (timer.ms() > 1500)
		move(MoveDirection::Down);
}

void BlockUnit::draw() const {

	const Point predictedPos(stdPos.movedBy(predictedPoint.y * blockSize, predictedPoint.x * blockSize));

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (geometry[i][j]) {
				geometry[i][j]->draw();
				Rect(predictedPos.movedBy(j*blockSize, i*blockSize), blockSize).drawFrame(blockSize / 10, 0.0);
			}
		}
	}

}

void BlockUnit::draw(const Point& pos, double scale) const {
	
	const int scaledSize = blockSize * scale;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (geometry[i][j]) {
				geometry[i][j]->draw(pos.movedBy(scaledSize*Point(j,i)), scale);
			}
		}
	}
}

void BlockUnit::fallImmediately() {
	point.set(predictedPoint);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (geometry[i][j])
				geometry[i][j]->setPoint(point.movedBy(i, j));
		}
	}
	settle();
}

void BlockUnit::move(MoveDirection mov) {
	
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

void BlockUnit::rotate(RotateDirection rot) {

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

void BlockUnit::predict() {
	predictedPoint.set(point.movedBy(-4, 0));
	while (!checkCollision(predictedPoint)) {
		predictedPoint.moveBy(1, 0);
	}
	predictedPoint.moveBy(-1, 0); //ここダサい
}

void BlockUnit::resetPoint() {
	point.set(0, constants::col_len / 2 - 2);
}*/