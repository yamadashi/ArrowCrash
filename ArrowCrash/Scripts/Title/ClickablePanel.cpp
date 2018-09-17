#include "ClickablePanel.h"


ClickablePanel::ClickablePanel(
	const int width,
	const int height,
	const Point& pos,
	const String& textureHandler_,
	const func& clickEvent_,
	const func& mouseOverEvent_,
	const func& mouseOutEvent_
)
	:Clickable(pos),
	shape(width, height),
	textureHandler(textureHandler_),
	clickEvent(clickEvent_),
	mouseOverEvent(mouseOverEvent_),
	mouseOutEvent(mouseOutEvent_)
{}

bool ClickablePanel::contains(const Point& point) const {
	return shape.movedBy(pos).contains(point);
}

void ClickablePanel::draw() const {
	if (textureHandler != L"") shape(TextureAsset(textureHandler)).draw(pos);
	else shape.movedBy(pos).draw(Palette::Lawngreen);
}