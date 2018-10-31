#pragma once
#include <Siv3d.hpp>

struct GameData {
	int blockSize; //ブロックのサイズ
	std::vector<Point> stdPositions; //各プレイヤーのフィールドの基準点
	std::vector<std::vector<Rect>>* nextUnitFrames;
	std::vector<Rect>* stockFrames;
};