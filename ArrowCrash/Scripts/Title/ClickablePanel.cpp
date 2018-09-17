#include "ClickablePanel.h"


ClickablePanel::ClickablePanel(int width, int height, Point pos, String texture_handler_, const func& clickEvent)
	:Clickable(clickEvent, pos),
	shape(width, height),
	texture_handler(texture_handler_)
{}

bool ClickablePanel::contains(const Point& point) const {
	return shape.movedBy(pos).contains(point);
}

void ClickablePanel::draw() const {
	if (texture_handler != L"") shape(TextureAsset(texture_handler)).draw(pos);
	else shape.movedBy(pos).draw(Palette::Lawngreen);
}