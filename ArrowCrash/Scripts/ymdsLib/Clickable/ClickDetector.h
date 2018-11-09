#pragma once
#include "IPointer.h"
#include "Clickable.h"

namespace ymds {

	class ClickDetector {
	private:
		std::vector<std::shared_ptr<IPointer>> pointers;
		std::vector<std::shared_ptr<Clickable>> targets;

	public:
		ClickDetector() = default;
		~ClickDetector() = default;

		void addTarget(std::shared_ptr<Clickable> target) { targets.emplace_back(target); }
		void clearTargets() { targets.clear(); }
		void addPointer(std::shared_ptr<IPointer> pointer) { pointers.emplace_back(pointer); }
		void clearPointers() { pointers.clear(); }

		void update();
	};

}