#pragma once
#include "Pointer.h"
#include "Clickable.h"

class ClickDetector {
private:
	std::vector<std::weak_ptr<Pointer>> pointers;
	std::vector<std::weak_ptr<Clickable>> targets;

public:
	ClickDetector() = default;
	~ClickDetector() = default;

	void addTarget(std::weak_ptr<Clickable> target) { targets.emplace_back(target); }
	void clearTargets() { targets.clear(); }
	void addPointer(std::weak_ptr<Pointer> pointer) { pointers.emplace_back(pointer); }
	void clearPointers() { pointers.clear(); }

	void update();
};
