#include "ClickDetector.h"


void ClickDetector::update() {

	for (auto&& target : targets) {
		Clickable& tgt = *target.lock();
		tgt.mouseOverCount = 0;

		for (const auto& pointer : pointers) {
			Pointer& ptr = *pointer.lock();

			if (tgt.contains(ptr.getPos()))
			{
				//マウスオーバー処理
				tgt.mouseOverCount++;
				if (!tgt.mouseOver) tgt.onMouseOver();

				//クリック処理
				if (ptr.isClicked()) {
					tgt.onClick();
					break;
				}
			}
		}
		if (tgt.mouseOver && tgt.mouseOverCount == 0) tgt.onMouseOut();

		tgt.mouseOver = tgt.mouseOverCount > 0;
	}
}