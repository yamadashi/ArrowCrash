#pragma once
#include <Siv3D.hpp>

namespace ymds {

	enum class GamepadIn {
		ONE, TWO, THREE, FOUR,
		L1, R1, L2, R2,
		SELECT, START,
		UP, RIGHT, DOWN, LEFT
	};
	const int NumOfGamepadInputType = 14;

	class Gamepad {
	private:
		s3d::Gamepad gamepad;
		std::array<bool, NumOfGamepadInputType> flags;
		std::array<bool, NumOfGamepadInputType> prevFlags;

	public:
		Gamepad(int index);
		~Gamepad() = default;

		void update();
		bool pressed(GamepadIn button);
		bool clicked(GamepadIn button);
		bool released(GamepadIn button);
	};

}