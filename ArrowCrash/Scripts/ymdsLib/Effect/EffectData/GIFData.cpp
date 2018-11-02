#include "GIFData.h"



ymds::AnimationGIFData::AnimationGIFData(const FilePath& path) {
	for (auto&& image : Imaging::LoadAnimatedGIF(path)) textures.emplace_back(image);
	size = textures.size();
}

ymds::GifReader::GifReader(const FilePath& path)
	:data(path),
	t(0)
{}

void ymds::GifReader::draw() {
	data.textures[t].draw();
	++t %= data.size;
}