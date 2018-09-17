#include "Clickablelabel.h"

ClickableLabel::ClickableLabel(
	const String& text_,
	const Point& centerPos,
	const String& fontHandler_,
	const Color& textColor,
	const func& clickEvent,
	const func& mouseOverEvent,
	const func& mouseOutEvent)

	:Clickable(clickEvent, mouseOverEvent, mouseOutEvent,
		centerPos.movedBy(-FontAsset(fontHandler_)(text_).region().size/2)),
	text(text_),
	fontHandler(fontHandler_),
	color(textColor)
{}


bool ClickableLabel::contains(const Point& point) const {
	return FontAsset(fontHandler)(text).region(pos).contains(point);
}

void ClickableLabel::draw() const {
	FontAsset(fontHandler)(text).draw(pos, color);
}