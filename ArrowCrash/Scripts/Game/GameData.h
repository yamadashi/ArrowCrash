#pragma once
#include <Siv3d.hpp>

struct GameData {
	std::vector<Point> stdPositions; //各プレイヤーのフィールドの基準点
	std::vector<std::vector<Point>> nextUnitFramePos;
	std::vector<Point> stockFramePos;
};