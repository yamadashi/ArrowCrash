#pragma once
#include <Siv3D.hpp>
#include "../Constants.h"
#include "../ymdsLib/GamePad/GamepadManager.h"


class Pointer {
private:
	int player_num;
	Vec2 pos;
	Vec2 velocity;
	const Color color;

	static const double accel;
	static const double frictionCoeff;
	static const double maxSpeed;

	const Size pointerSize;
	int anim_t; //アニメーション用
	const int anim_coeff;
	const int anim_num;
	const int anim_size;

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
