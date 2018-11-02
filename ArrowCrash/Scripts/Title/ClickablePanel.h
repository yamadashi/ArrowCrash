#pragma once
#include "Clickable.h"
#include <Siv3D.hpp>

//ëOï˚êÈåæ
class ClickablePanel;

using func = std::function<void(ClickablePanel&)>;


class ClickablePanel : public Clickable {
private:
	String textureHandler;
	Rect shape;
	std::function<void(ClickablePanel&)> clickEvent;
	std::function<void(ClickablePanel&)> mouseOverEvent;
	std::function<void(ClickablePanel&)> mouseOutEvent;

public:
	ClickablePanel(
		const String& texture_handler,
		const Point& pos,
		const Size size,
		const func& clickEvent = [](Clickable&) {},
		const func& mouseOverEvent = [](Clickable&) {},
		const func& mouseOutEvent = [](Clickable&) {});
	ClickablePanel(
		const String& texture_handler,
		const Point& pos,
		const double scale = 1.0,
		const func& clickEvent = [](Clickable&) {},
		const func& mouseOverEvent = [](Clickable&) {},
		const func& mouseOutEvent = [](Clickable&) {});
	~ClickablePanel() = default;

	void setTextureHandler(const String& handler) { textureHandler = handler; }

	void onClick() override { clickEvent(*this); }
	void onMouseOver() override { mouseOverEvent(*this); }
	void onMouseOut() override { mouseOutEvent(*this); }

	bool contains(const Point& point) const override;
	void draw() const override;
};