#include "Clickablelabel.h"

ClickableLabel::ClickableLabel(
	const String& text_,
	const String& fontHandler_,
	const Point& centerPos,
	const Color& textColor,
	const std::function<void(ClickableLabel&)>& clickEvent_,
	const std::function<void(ClickableLabel&)>& mouseOverEvent_,
	const std::function<void(ClickableLabel&)>& mouseOutEvent_
)
	:Clickable(centerPos.movedBy(-FontAsset(fontHandler_)(text_).region().size/2)),
	text(text_),
	fontHandler(fontHandler_),
	color(textColor),
	clickEvent(clickEvent_),
	mouseOverEvent(mouseOverEvent_),
	mouseOutEvent(mouseOutEvent_)
{}


bool ClickableLabel::contains(const Point& point) const {
	return FontAsset(fontHandler)(text).region(pos).contains(point);
}

void ClickableLabel::draw() const {
	FontAsset(fontHandler)(text).draw(pos, color);
}