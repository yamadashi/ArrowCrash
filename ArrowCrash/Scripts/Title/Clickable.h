#pragma once
#include <functional>
#include <Siv3D.hpp>

//ëOï˚êÈåæ
class Clickable;

using func = std::function<void(Clickable&)>;

class Clickable {
private:
	func clickEvent;
	func mouseOverEvent;
	func mouseOutEvent;

protected:
	Point pos;

public:
	bool mouseOver;

	Clickable() = default;
	Clickable(
		const func& clickEvent_,
		const func& mouseOverEvent_,
		const func& mouseOutEvent_,
		const Point& pos_)

		:clickEvent(clickEvent_),
		mouseOverEvent(mouseOverEvent_),
		mouseOutEvent(mouseOutEvent_),
		pos(pos_)
	{}
	virtual ~Clickable() = default;

	void setOnClickEvent(const func& clickEvent_) {
		clickEvent = clickEvent_;
	}
	void setOnMouseOverEvent(const func& mouseOverEvent_) {
		mouseOverEvent = mouseOverEvent_;
	}
	void setOnMouseOutEvent(const func& mouseOutEvent_) {
		mouseOutEvent = mouseOutEvent_;
	}

	void moveBy(const Point& delta) { pos.moveBy(delta); }
	void setPos(const Point& newPos) { pos.set(newPos); }

	void onClick() { clickEvent(*this); }
	void onMouseOver() { mouseOverEvent(*this); }
	void onMouseOut() { mouseOutEvent(*this); }
	virtual bool contains(const Point& point) const = 0;
	virtual void draw() const = 0;
};