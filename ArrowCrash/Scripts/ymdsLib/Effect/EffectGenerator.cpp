#include "EffectGenerator.h"



class ymds::EffectGenerator::EffectEvent : public ymds::Event {
private:
	std::unique_ptr<Strategy> strategy;

public:
	EffectEvent(const FilePath& gifPath, const Point& LeftUpPos, double scale, double speedRatio); //GIFアニメ版のコンストラクタ
	EffectEvent(const AnimationGIFData& data, const Point& LeftUpPos, double scale, double speedRatio); //GIFアニメ版のコンストラクタ
	EffectEvent(const String textureHandler, const size_t cellWidth, const Point& LeftUpPos, double scale, double speedRatio); //連結画像版のコンストラクタ
	void update() override { if (strategy->update()) kill(); }
	void draw() const override { strategy->draw(); }

};


ymds::EffectGenerator::EffectEvent::EffectEvent(const FilePath& path, const Point& leftUpPos, double scale, double speedRatio) 
	:strategy(new AnimationGIFStrategy(path, leftUpPos, scale, speedRatio))
{}

ymds::EffectGenerator::EffectEvent::EffectEvent(const AnimationGIFData& data, const Point& leftUpPos, double scale, double speedRatio)
	: strategy(new AnimationGIFStrategy(data, leftUpPos, scale, speedRatio))
{}

ymds::EffectGenerator::EffectEvent::EffectEvent(const String textureHandler, const size_t cellWidth, const Point& LeftUpPos, double scale, double speedRatio)
	: strategy(new LinkedImageStrategy(textureHandler, cellWidth, LeftUpPos, scale, speedRatio))
{}


void ymds::EffectGenerator::addGIF(const FilePath& path, const Point& leftUpPos, double scale, double speedRatio) {
	ymds::EventManager::get().registerEvent(new EffectEvent(path, leftUpPos, scale, speedRatio));
}

void ymds::EffectGenerator::addGIF(const AnimationGIFData& data, const Point& leftUpPos, double scale, double speedRatio) {
	ymds::EventManager::get().registerEvent(new EffectEvent(data, leftUpPos, scale, speedRatio));
}

void ymds::EffectGenerator::addLinkedImage(const String textureHandler, const size_t cellWidth, const Point& LeftUpPos, double scale, double speedRatio) {
	ymds::EventManager::get().registerEvent(new EffectEvent(textureHandler, cellWidth, LeftUpPos, scale, speedRatio));
}