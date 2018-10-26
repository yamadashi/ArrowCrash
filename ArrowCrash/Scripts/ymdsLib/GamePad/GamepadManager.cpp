#include "GamepadManager.h"


ymds::GamepadManager::GamepadManager()
	:gamepads(),
	active(true)
{
	for (int i = 0; i < 4; i ++) gamepads.emplace_back(i);
}

void ymds::GamepadManager::update() {

	if (!active) return;

	for (auto& gamepad : gamepads) gamepad.update();
	
}

bool ymds::GamepadManager::any(std::function<bool(ymds::Gamepad& gamepad)> pred) {
	return std::any_of(gamepads.begin(), gamepads.end(), pred);
}