#pragma once
#include "../Scene.h"
#include "../ymdsLib/GamePad/GamepadManager.h"


class Result : public Scene {
private:
	

public:
	Result();
	~Result();

	void init() override;
	void update() override;
	void draw() const override;
};