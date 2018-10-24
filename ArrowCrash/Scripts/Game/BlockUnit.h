#pragma once
#include "Block.h"
#include "Field.h"

constexpr bool unitPatterns[7][4][4] = {
	{
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 1, 1, 1 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 1, 1, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 1, 1 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 1, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 1, 1, 0 }
	},
};

//enum class UnitType {
//	I,T,Z,S,O,L,J
//};

enum class MoveDirection {
	Left, Right, Down
};

using namespace std;

class BlockUnit {
private:
	std::array<std::array<std::shared_ptr<Block>, 4>, 4> geometry;

	Field& field;
	Point point;
	Point predictedPoint;
	bool settled;
	Stopwatch timer;
	const int blockSize;
	const Point stdPos; //フィールド基準点
	const double arrowProbability;
	std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks;

	bool checkCollision(const Point& point_) const;
	void settle();
	int countNumOfBlock(const bool pattern[4][4]) const;

public:
	BlockUnit(const Point& point_, const Point& stdPos_, const int blockSize_, std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks, Field& field_);
	~BlockUnit() = default;

	void update();
	void draw() const;
	void draw(const Point& pos, double scale) const;
	void fallImmediately();
	void move(MoveDirection);
	void rotate(RotateDirection);
	bool isSettled() const { return settled; }
	void predict();
	void resetPoint();
};