#include "BlockUnit.h"

BlockUnit::BlockUnit(const Point& point_, const Point& stdPos_, const int blockSize_, std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks_, Field& field_)
	:point(point_),
	field(field_),
	settled(false),
	timer(true),
	blockSize(blockSize_),
	arrowBlocks(arrowBlocks_),
	stdPos(stdPos_),
	arrowProbability(0.15)
{
	auto& pattern = unitPatterns[Random<int>(0, 6)];
	
	//ArrowBlock生成(分かり辛くてごめんやで)
	int arrowOrder;
	arrowOrder = Random() < arrowProbability ? Random<int>(0, countNumOfBlock(pattern) - 1) : -1;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (pattern[i][j]) {
				if (arrowOrder-- == 0) {
					auto ptr = std::make_shared<ArrowBlock>(point.movedBy(i, j), stdPos, blockSize, (ExplosionDirection)Random<int>(0, 7), field);
					geometory[i][j] = ptr;
					arrowBlocks.emplace_back(ptr);
				}
				else
					geometory[i][j] = std::make_shared<NormalBlock>(point.movedBy(i, j), stdPos, blockSize);
			}
			else {
				geometory[i][j] = nullptr;
			}
		}
	}
	predict();
}



bool BlockUnit::checkCollision(const Point& point_) const {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (field.getAt(point_.movedBy(i, j)) && geometory[i][j])
				return true;
		}
	}
	return false;
}

void BlockUnit::predict() {
	predictedPoint.set(point);
	while (!checkCollision(predictedPoint)) {
		predictedPoint.moveBy(1, 0);
	}
	predictedPoint.moveBy(-1, 0); //ここダサい
}

void BlockUnit::settle() {

	for (auto&& arr : geometory) {
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
			if (geometory[i][j]) {
				geometory[i][j]->draw();
				Rect(predictedPos.movedBy(j*blockSize, i*blockSize), blockSize).drawFrame(blockSize / 10, 0.0);
			}
		}
	}
	PutText(predictedPoint).from(stdPos);
}


void BlockUnit::fallImmediately() {
	point.set(predictedPoint);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (geometory[i][j])
				geometory[i][j]->setPoint(point.movedBy(i, j));
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
			if (geometory[i][j])
				geometory[i][j]->setPoint(point.movedBy(i, j));
		}
	}

}

void BlockUnit::rotate(RotateDirection rot) {

	auto prev = geometory;
	
	if (rot == RotateDirection::Right) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				geometory[j][3 - i] = prev[i][j];
			}
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				geometory[3 - j][i] = prev[i][j];
			}
		}
	}

	if (checkCollision(point)) {
		geometory = prev;
		return;
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (geometory[i][j]) {
				geometory[i][j]->setPoint(point.movedBy(i, j));
				geometory[i][j]->rotate(rot);
			}
		}
	}
	predict();

}