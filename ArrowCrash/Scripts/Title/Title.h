#pragma once
#include "../Scene.h"
#include "../Pointer.h"
#include "../ymdsLib/Clickable/ClickableLabel.h"
#include "../ymdsLib/Clickable/ClickablePanel.h"
#include "../ymdsLib/Clickable/IPointer.h"
#include "../ymdsLib/Clickable/ClickDetector.h"

class Title : public Scene {
private:
	ymds::ClickDetector clickDetector;
	std::vector<std::shared_ptr<ymds::IPointer>> pointers;
	std::vector<std::shared_ptr<ymds::Clickable>> targets;

	enum class TitleScene { TOP, SELECT } scene;
	bool transition;

	const int maxSpeed;
	double speed;
	const double deceleration;
	Point selectViewPos;
	float alpha; //•s“§–¾“x

public:
	Title();
	~Title();
	void init() override;
	void update() override;
	void draw() const override;
};