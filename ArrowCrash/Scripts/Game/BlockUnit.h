#pragma once
#include "Block.h"
#include "Field.h"

constexpr bool unitPatterns[7][4][4] = {
	{
		{ 0, 0, 0, 0 },
		{ 1, 1, 1, 1 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 1, 1, 1 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 1, 1 },
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
		{ 0, 1, 1, 1 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 1, 1, 1 },
		{ 0, 0, 0, 1 },
		{ 0, 0, 0, 0 }
	},
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
	const Point stdPos; //フィールド基準点

	int speed;
	const int normalSpeed;
	const int highSpeed;
	bool cannotRotate;

	static const double arrowProbability;

	bool checkCollision(const Point& point_) const;
	void settle();
	int countNumOfBlock(const bool pattern[4][4]) const;

public:
	Unit(const Point& point_, const Point& stdPos_, Field& field_);
	~Unit() = default;

	virtual void update() = 0;
	virtual void draw() const = 0;
	virtual void draw(const Point& pos, double scale) const = 0;
	virtual void fallImmediately();
	virtual void move(MoveDirection);
	virtual void rotate(RotateDirection);
	void predict();
	bool isSettled() const { return settled; }
	void resetPoint();
	bool cannotSettle();
	void restartTimer() { timer.restart(); }
	void changeSpeed(bool act);
	void changeForbid(bool act);
	virtual bool ItemCheck() { return false; }
	Point getUnitCenter();
};

class BlockUnit : public Unit {
protected:
	
	const double arrowProbability;
	std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks;
	const UnitType type;

public:
	BlockUnit(const Point& point_, const Point& stdPos_, std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks, Field& field_);
	virtual ~BlockUnit() = default;

	void update() override;
	void draw() const override;
	void draw(const Point& pos, double scale) const override;
};
