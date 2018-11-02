#pragma once
#include "../Scene.h"
#include "../ymdsLib/GamePad/GamepadManager.h"


class Result : public Scene {
private:
	

public:
	Result();
	~Result();

	void update() override;
	void draw() const;
};