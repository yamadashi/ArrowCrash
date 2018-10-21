#include "EffectStrategy.h"



ymds::AnimationGIFStrategy::AnimationGIFStrategy(const FilePath& path, const Point& pos, double scale, double speedRatio)
	:Strategy(pos, scale, speedRatio),
	data(path), //�]���R���X�g���N�^
	t(0)
{}

ymds::AnimationGIFStrategy::AnimationGIFStrategy(const AnimationGIFData& data, const Point& pos, double scale, double speedRatio)
	: Strategy(pos, scale, speedRatio),
	data(data),
	t(0)
{}

bool ymds::AnimationGIFStrategy::update() {
	if (++t / speedCoeff >= data.size) return true;
	else return false;
}

void ymds::AnimationGIFStrategy::draw() const {
	data.textures[size_t(t / speedCoeff)].scale(scale).draw(leftUpPos);
}


ymds::LinkedImageStrategy::LinkedImageStrategy(const String textureHandler_, const size_t cellWidth_, const Point& pos, double scale, double speedRatio)
	:Strategy(pos, scale, speedRatio),
	textureHandler(textureHandler_),
	counter(0),
	cellWidth(cellWidth_),
	size(TextureAsset(textureHandler).width / cellWidth)
{}

bool ymds::LinkedImageStrategy::update() {
	if (size_t(++counter / speedCoeff) >= size) return true;
	else return false;
}

void ymds::LinkedImageStrategy::draw() const {
	size_t index = size_t(counter / speedCoeff);
	auto&& tmp = TextureAsset(textureHandler);
	tmp(cellWidth*index, 0, cellWidth, tmp.height).scale(scale).draw(leftUpPos);
}