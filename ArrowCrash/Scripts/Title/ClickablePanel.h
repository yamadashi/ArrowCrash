#pragma once
#include "Clickable.h"
#include <Siv3D.hpp>

class ClickablePanel : public Clickable {
private:
	Rect shape;
	String textureHandler;

public:
	ClickablePanel(
		const int width,
		const int height,
		const Point& pos,
		const String& texture_handler = L"",
		const func& clickEvent = [](Clickable&) {},
		const func& mouseOverEvent = [](Clickable&) {},
		const func& mouseOutEvent = [](Clickable&) {});
	~ClickablePanel() = default;

	void setTextureHandler(const String& handler) { textureHandler = handler; }

	bool contains(const Point& point) const override;
	void draw() const override;
};