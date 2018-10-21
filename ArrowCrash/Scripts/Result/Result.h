#pragma once
#include "../Scene.h"


class Result : public Scene {
private:
	

public:
	Result();
	~Result();

	void update() override;
	void draw() const;
};