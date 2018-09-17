#pragma once
#include "Clickable.h"
#include <Siv3D.hpp>

class ClickablePanel : public Clickable {
private:
	Rect shape;
	String texture_handler;

public:
	ClickablePanel(int width, int height, Point pos, String texture_handler = L"", const func& clickEvent = [](Clickable&) {});
	~ClickablePanel() = default;

	bool contains(const Point& point) const override;
	void draw() const override;
};