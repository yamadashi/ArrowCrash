#pragma once
#include <Siv3D.hpp>
#include "Field.h"
#include "Blockunit.h"

using namespace std;

class ItemUnit : public Unit {
private:
	bool destroyed;
	int SpeedUpRate;
	int ForbidRotatingRate;
	int InterruptionGuardRate;

public:
	ItemUnit(const Point& point, const Point& stdPos, Field& field_);
	virtual ~ItemUnit() = default;

	void update() override;
	void draw() const override;
	void draw(const Point& pos, double scale) const {}
	void fallImmediately() override {}
	void move(MoveDirection) override {}
	void rotate(RotateDirection) override {}
	void destroy() { destroyed = true; }
	bool isDestroyed() const { return destroyed; }
	bool ItemCheck() override { return true; }
};