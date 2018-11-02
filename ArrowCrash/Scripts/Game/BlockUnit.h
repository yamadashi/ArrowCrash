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


class Unit {
protected:
	std::array<std::array<std::shared_ptr<Block>, 4>, 4> geometry;

	Field& field;
	Point point;
	Point predictedPoint;
	bool settled;
	Stopwatch timer;
	const int blockSize;
	const Point stdPos; //フィールド基準点

	bool checkCollision(const Point& point_) const;
	void settle();
	int countNumOfBlock(const bool pattern[4][4]) const;

public:
	Unit(const Point& point_, const Point& stdPos_, const int blockSize_, Field& field_);
	virtual ~Unit() = default;

	virtual void update();
	virtual void draw() const;
	virtual void draw(const Point& pos, double scale) const;
	virtual void fallImmediately() {}
	virtual void move(MoveDirection) {}
	virtual void rotate(RotateDirection) {}
	bool isSettled() const { return settled; }
	virtual void predict() {}
	void resetPoint();
	bool checkStackedFully();
	void restartTimer() { timer.restart(); }
};

class BlockUnit : public Unit {
protected:
	
	const double arrowProbability;
	std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks;

public:
	BlockUnit(const Point& point_, const Point& stdPos_, const int blockSize_, std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks, Field& field_);
	virtual ~BlockUnit() = default;

	void fallImmediately() override;
	void move(MoveDirection) override;
	void rotate(RotateDirection) override;
	void predict() override;
};

/*
class BlockUnit {
	protected:
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
	virtual ~BlockUnit() = default;

	void update();
	void draw() const;
	void draw(const Point& pos, double scale) const;
	void fallImmediately();
	void move(MoveDirection);
	void rotate(RotateDirection);
	bool isSettled() const { return settled; }
	void predict();
	void resetPoint();
<<<<<<< HEAD
	bool checkStackedFully();
	void restartTimer() { timer.restart(); }
};
=======
};*/
