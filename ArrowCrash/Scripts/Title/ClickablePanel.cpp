#include "ClickablePanel.h"


ClickablePanel::ClickablePanel(
	const int width,
	const int height,
	const Point& pos,
	const String& textureHandler_,
	const func& clickEvent,
	const func& mouseOverEvent,
	const func& mouseOutEvent)

	:Clickable(clickEvent, mouseOverEvent, mouseOutEvent, pos),
	shape(width, height),
	textureHandler(textureHandler_)
{}

bool ClickablePanel::contains(const Point& point) const {
	return shape.movedBy(pos).contains(point);
}

void ClickablePanel::draw() const {
	if (textureHandler != L"") shape(TextureAsset(textureHandler)).draw(pos);
	else shape.movedBy(pos).draw(Palette::Lawngreen);
}