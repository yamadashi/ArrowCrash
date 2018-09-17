#pragma once
#include "Clickable.h"
#include <Siv3D.hpp>


class ClickableLabel : public Clickable {
private:
	String text;
	String font_handler;

public:
	ClickableLabel(String text_, Point centerPos, String font_handler_, const func& clickEvent = [](Clickable&) {});
	~ClickableLabel() = default;
	
	bool contains(const Point& point) const override;
	void draw() const override;
};