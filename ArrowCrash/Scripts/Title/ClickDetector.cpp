#include "ClickDetector.h"


void ClickDetector::update() {

	for (auto&& target : targets) {
		for (const auto& pointer : pointers) {
			if (pointer.lock()->isClicked() && target.lock()->contains(pointer.lock()->getPos()))
			{
				target.lock()->onClick();
				break;
			}
		}
	}
}