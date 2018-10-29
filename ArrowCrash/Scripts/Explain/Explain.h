#pragma once
#include "../Scene.h"

class Explain : public Scene {
private:

public:
	void init() override;
	void update() override;
	void draw() const override;

};