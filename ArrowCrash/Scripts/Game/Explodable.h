#pragma once
#include <Siv3D.hpp>


enum class ExplosionDirection {
	Up, UpRight, Right, DownRight, Down, DownLeft, Left, UpLeft
};

class Explodable {
public:
	virtual int explode(const Point& point, ExplosionDirection direction) = 0;
};