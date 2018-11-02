#pragma once
#include "../EventSystem/EventSystem.h"
#include "EffectData/EffectStrategy.h"


namespace ymds {

	class EffectGenerator {
	private:
		class EffectEvent;

	public:
		static void addGIF(const FilePath& path, const Point& LeftUpPos, double scale = 1.0, double speedRatio = 1.0);
		static void addGIF(const AnimationGIFData& data, const Point& leftUpPos, double scale = 1.0, double speedRatio = 1.0);
		static void addLinkedImage(const String textureHandler, const size_t cellWidth, const Point& LeftUpPos, double scale = 1.0, double speedRatio = 1.0);
	};
}