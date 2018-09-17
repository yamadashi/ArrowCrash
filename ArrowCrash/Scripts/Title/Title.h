#pragma once
#include "../Scene.h"
#include "Pointer.h"
#include "ClickableLabel.h"
#include "ClickablePanel.h"
#include "ClickDetector.h"

enum class TitleScene { TOP, SELECT };

class Title : public Scene {
private:
	ClickDetector clickDetector;
	std::shared_ptr<Pointer> pointer;
	std::vector<std::shared_ptr<Clickable>> targets;

	TitleScene scene;
	bool transition;

public:
	Title();
	~Title() = default;

	void update();

	void draw() const;
};