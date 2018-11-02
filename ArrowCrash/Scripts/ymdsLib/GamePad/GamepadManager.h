#pragma once
#include <Siv3D.hpp>
#include "Gamepad.h"

namespace ymds {

	/* ƒVƒ“ƒOƒ‹ƒgƒ“ */
	class GamepadManager {
	private:
		GamepadManager();
		GamepadManager(const GamepadManager&) {}
		GamepadManager& operator=(const GamepadManager&) {}

		std::vector<ymds::Gamepad> gamepads;
		bool active;

	public:
		static GamepadManager& get() {
			static GamepadManager instance;
			return instance;
		}

		void update();
		ymds::Gamepad& getGamepad(int index) { return gamepads.at(index); }
		bool any(std::function<bool(ymds::Gamepad& gamepad)> pred);

		void activate() { active = true; }
		void inactivate() { active = false; }

	};
}