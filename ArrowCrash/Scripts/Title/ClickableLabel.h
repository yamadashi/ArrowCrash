#pragma once
#include "Clickable.h"
#include <Siv3D.hpp>


class ClickableLabel : public Clickable {
private:
	String text;
	String fontHandler;
	Color color;
	std::function<void(ClickableLabel&)> clickEvent;
	std::function<void(ClickableLabel&)> mouseOverEvent;
	std::function<void(ClickableLabel&)> mouseOutEvent;

public:
	ClickableLabel(
		const String& text_,
		const Point& centerPos,
		const String& font_handler_,
		const Color& textColor = Palette::Darkslategray,
		const std::function<void(ClickableLabel&)>& clickEvent = [](ClickableLabel&) {},
		const std::function<void(ClickableLabel&)>& mouseOverEvent = [](ClickableLabel&) {},
		const std::function<void(ClickableLabel&)>& mouseOutEvent = [](ClickableLabel&) {});
	~ClickableLabel() = default;

	void setText(const String& txt) { text = txt; }
	void setColor(const Color& clr) { color = clr; }

	void onClick() override { clickEvent(*this); }
	void onMouseOver() override { mouseOverEvent(*this); }
	void onMouseOut() override { mouseOutEvent(*this); }
	
	bool contains(const Point& point) const override;
	void draw() const override;
};