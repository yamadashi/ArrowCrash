#pragma once
#include "../Scene.h"
#include "../Pointer.h"
#include "../ymdsLib/GamePad/GamepadManager.h"
#include "../ymdsLib/Clickable/ClickablePanel.h"
#include "../ymdsLib/Clickable/IPointer.h"
#include "../ymdsLib/Clickable/ClickDetector.h"

class Explain : public Scene {
private:
	ymds::ClickDetector clickDetector;
	std::vector<std::shared_ptr<ymds::IPointer>> pointers;
	std::vector<std::shared_ptr<ymds::Clickable>> targets;
	int counter;
	const int numOfExplain;

public:
	Explain();
	~Explain();

	void init() override;
	void update() override;
	void draw() const override;

};