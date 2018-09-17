#pragma once
#include <Siv3D.hpp>


enum class ExplosionDirection {
	Up, Down, Left, Right, UpLeft, UpRight, DownLeft, DownRight
};

class Explodable {
public:
	virtual void explode(const Point& point, ExplosionDirection direction) = 0;
};