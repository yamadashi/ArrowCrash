#pragma once
#include <functional>
#include <Siv3D.hpp>

class Clickable {
protected:
	Point pos;

public:
	int mouseOverCount;
	bool mouseOver;

	Clickable() = default;
	Clickable(const Point& pos_)
		:pos(pos_),
		mouseOver(false),
		mouseOverCount(0)
	{}
	virtual ~Clickable() = default;

	void moveBy(const Point& delta) { pos.moveBy(delta); }
	void setPos(const Point& newPos) { pos.set(newPos); }

	virtual void onClick() = 0;
	virtual void onMouseOver() = 0;
	virtual void onMouseOut() = 0;

	virtual bool contains(const Point& point) const = 0;
	virtual void draw() const = 0;
};