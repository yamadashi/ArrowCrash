#include "Clickablelabel.h"

ClickableLabel::ClickableLabel(String text_, Point centerPos, String font_handler_, const func& clickEvent) 
	:Clickable(clickEvent, centerPos.movedBy(-FontAsset(font_handler_)(text_).region().size/2)),
	text(text_),
	font_handler(font_handler_)
{}


bool ClickableLabel::contains(const Point& point) const {
	return FontAsset(font_handler)(text).region(pos).contains(point);
}

void ClickableLabel::draw() const {
	FontAsset(font_handler)(text).draw(pos, Palette::Darkslategray);
}