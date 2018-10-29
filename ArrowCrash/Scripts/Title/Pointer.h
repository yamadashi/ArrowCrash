#pragma once
#include <Siv3D.hpp>
#include "../Constants.h"
#include "../ymdsLib/GamePad/GamepadManager.h"


class Pointer {
private:
	Vec2 pos;
	Vec2 velocity;
	int player_num;
	const Color color;

	static const int radius;
	static const double accel;
	static const double frictionCoeff;
	static const double maxSpeed;

	void move();
	bool isOutOfRegion() const;

public:
	Pointer(int player_num_);
	~Pointer() = default;

	void update();
	void draw() const;

	bool isClicked() const;
	Point getPos() const;
};
