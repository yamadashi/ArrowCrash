#pragma once
#include "../Scene.h"
#include "Pointer.h"
#include "ClickableLabel.h"
#include "ClickablePanel.h"
#include "ClickDetector.h"

class Title : public Scene {
private:
	ClickDetector clickDetector;
	std::shared_ptr<Pointer> pointer;
	std::vector<std::shared_ptr<Clickable>> targets;

	enum class TitleScene { TOP, SELECT } scene;
	bool transition;

	const int maxSpeed;
	double speed;
	const double deceleration;
	Point selectViewPos;

public:
	Title();
	~Title() = default;

	void update();

	void draw() const;
};