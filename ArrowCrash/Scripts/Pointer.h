#pragma once
#include <Siv3D.hpp>
#include "ymdsLib/Clickable/IPointer.h"
#include "ymdsLib/GamePad/GamepadManager.h"
#include "Constants.h"

class Pointer : public ymds::IPointer {
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
	Pointer(int player_num_, const Point& pos_);
	~Pointer() = default;

	void update() override;
	void draw() const override;

	bool isClicked() const override;
	Point getPos() const override;
	void setPos(const Point& pos_) { pos.set(pos_); }
};