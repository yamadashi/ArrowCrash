#pragma once
#include "Clickable.h"
#include <Siv3D.hpp>


class ClickableLabel : public Clickable {
private:
	String text;
	String fontHandler;
	Color color;

public:
	ClickableLabel(
		const String& text_,
		const Point& centerPos,
		const String& font_handler_,
		const Color& textColor = Palette::Darkslategray, //‰¼
		const func& clickEvent = [](Clickable&) {},
		const func& mouseOverEvent = [](Clickable&) {},
		const func& mouseOutEvent = [](Clickable&) {});
	~ClickableLabel() = default;

	void setText(const String& txt) { text = txt; }
	void setColor(const Color& clr) { color = clr; }
	
	bool contains(const Point& point) const override;
	void draw() const override;
};