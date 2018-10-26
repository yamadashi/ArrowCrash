#include "GamePadManager.h"


ymds::GamePadManager::GamePadManager()
	:gamepads(),
	active(true)
{
	for (int i = 0; i < 4; i ++) gamepads.emplace_back(i);
}

void ymds::GamePadManager::update() {

	if (!active) return;

	for (auto& gamepad : gamepads) gamepad.update();
	
}