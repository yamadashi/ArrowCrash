#include "ClickDetector.h"


void ClickDetector::update() {

	for (auto&& target : targets) {
		for (const auto& pointer : pointers) {
			Clickable& tgt = *target.lock();
			Pointer& ptr = *pointer.lock();

			if (tgt.contains(ptr.getPos()))
			{
				//マウスオーバー処理
				if (!tgt.mouseOver) {
					tgt.mouseOver = true;
					tgt.onMouseOver();
				}
				//クリック処理
				if (ptr.isClicked()) {
					tgt.onClick();
					break;
				}
			} 
			else if (tgt.mouseOver) {
				tgt.mouseOver = false;
				tgt.onMouseOut();
			}
		}
	}
}