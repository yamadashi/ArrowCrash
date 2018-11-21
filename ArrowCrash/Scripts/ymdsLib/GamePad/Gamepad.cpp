#include "Gamepad.h"



ymds::Gamepad::Gamepad(int index)
	:gamepad(index),
	flags(),
	prevFlags()
{
	for (int i = 0; i < NumOfGamepadInputType; i++) {
		flags.at(i) = prevFlags.at(i) = false;
	}
}

void ymds::Gamepad::update() {
	
	for (int i = 0; i < NumOfGamepadInputType; i++) prevFlags.at(i) = flags.at(i);

	for (int i = 0; i < 10; i++) {
		flags.at(i) = gamepad.button(i).pressed;
	}
	
	flags.at((int)GamepadIn::UP) = (gamepad.r < -0.8f || gamepad.y < -0.8f || gamepad.povForward.pressed);
	flags.at((int)GamepadIn::DOWN) = (gamepad.r > 0.8f || gamepad.y > 0.8f || gamepad.povBackward.pressed);
	flags.at((int)GamepadIn::LEFT) = (gamepad.z < -0.8f || gamepad.x < -0.8f || gamepad.povLeft.pressed);
	flags.at((int)GamepadIn::RIGHT) = (gamepad.z > 0.8f || gamepad.x > 0.8f || gamepad.povRight.pressed);

	////キーボード入力
	flags.at((int)GamepadIn::TWO) = Input::Key2.pressed;
	flags.at((int)GamepadIn::THREE) = Input::Key3.pressed;
	flags.at((int)GamepadIn::START) = Input::KeyP.pressed;
	flags.at((int)GamepadIn::L1) = Input::KeyL.pressed;
	flags.at((int)GamepadIn::R1) = Input::KeyR.pressed;
	flags.at((int)GamepadIn::UP) = Input::KeyUp.pressed;
	flags.at((int)GamepadIn::RIGHT) = Input::KeyRight.pressed;
	flags.at((int)GamepadIn::DOWN) = Input::KeyDown.pressed;
	flags.at((int)GamepadIn::LEFT) = Input::KeyLeft.pressed;
}

bool ymds::Gamepad::pressed(GamepadIn button) {
	return flags.at((int)button);
}

bool ymds::Gamepad::clicked(GamepadIn button) {
	return flags.at((int)button) && !prevFlags.at((int)button);
}

bool ymds::Gamepad::released(GamepadIn button) {
	return !flags.at((int)button) && prevFlags.at((int)button);
}