#pragma once
#include <functional>
#include <Siv3D.hpp>

//ëOï˚êÈåæ
class Clickable;

using func = std::function<void(Clickable&)>;

class Clickable {
private:
	func clickEvent;

protected:
	Point pos;

public:
	Clickable() = default;
	Clickable(const func& clickEvent_, const Point& pos_)
		:clickEvent(clickEvent_),
		pos(pos_)
	{}
	virtual ~Clickable() = default;

	virtual void setOnClickEvent(const func& clickEvent_) {
		clickEvent = clickEvent_;
	}

	void moveBy(const Point& delta) { pos.moveBy(delta); }
	void setPos(const Point& newPos) { pos.set(newPos); }

	void onClick() { clickEvent(*this); }
	virtual bool contains(const Point& point) const = 0;
	virtual void draw() const = 0;
};