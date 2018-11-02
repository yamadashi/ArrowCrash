#include "ClickablePanel.h"


ClickablePanel::ClickablePanel(
	const String& textureHandler_,
	const Point& pos,
	const Size size,
	const func& clickEvent_,
	const func& mouseOverEvent_,
	const func& mouseOutEvent_
)
	:Clickable(pos),
	textureHandler(textureHandler_),
	shape(size),
	clickEvent(clickEvent_),
	mouseOverEvent(mouseOverEvent_),
	mouseOutEvent(mouseOutEvent_)
{}

ClickablePanel::ClickablePanel(
	const String& textureHandler_,
	const Point& pos,
	const double scale,
	const func& clickEvent_,
	const func& mouseOverEvent_,
	const func& mouseOutEvent_
)
	:Clickable(pos),
	textureHandler(textureHandler_),
	shape((scale*Vec2(TextureAsset(textureHandler).size)).asPoint()),
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