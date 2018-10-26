#pragma once
#include <Siv3D.hpp>
#include "Gamepad.h"

namespace ymds {

	/* ƒVƒ“ƒOƒ‹ƒgƒ“ */
	class GamePadManager {
	private:
		GamePadManager();
		GamePadManager(const GamePadManager&) {}
		GamePadManager& operator=(const GamePadManager&) {}

		std::vector<ymds::Gamepad> gamepads;
		bool active;

	public:
		static GamePadManager& get() {
			static GamePadManager instance;
			return instance;
		}

		void update();
		ymds::Gamepad& getGamepad(int index) { return gamepads.at(index); }

		void activate() { active = true; }
		void inactivate() { active = false; }

	};
}