#pragma once
#include <Siv3D.hpp>
#include "Field.h"
#include "Blockunit.h"

using namespace std;

class ItemUnit : public Unit {
private:
	bool destroyed;

public:
	ItemUnit(const Point& point, const Point& stdPos, Field& field_);
	virtual ~ItemUnit() = default;

	void update() override;
	void draw() const override;
	void draw(const Point& pos, double scale) const {}
	virtual void activateEffect() {};
	void fallImmediately() override {}
	void move(MoveDirection) override {}
	void rotate(RotateDirection) override {}
	void destroy() { destroyed = true; }
	bool isDestroyed() const { return destroyed; }
};
/*
//âÒì]ã÷é~
class ForbidRotating : public ItemUnit {
public:
	ForbidRotating(const Point& point_, const Point& stdPos, Field& field_);
	~ForbidRotating() = default;

	void activateEffect() override {}
};


//óéâ∫ë¨ìxè„è∏
class IncreaseFallVelocity : public ItemUnit {
public:
	IncreaseFallVelocity(const Point& point_, const Point& stdPos, Field& field_);
	~IncreaseFallVelocity() = default;

	void activateEffect() override {}
};


//Ç®é◊ñÇñ≥å¯
class InvalidateInterruption : public ItemUnit {
public:
	InvalidateInterruption(const Point& point_, const Point& stdPos, Field& field_);
	~InvalidateInterruption() = default;

	void activateEffect() override {}
};*/