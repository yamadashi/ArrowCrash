#pragma once
#include <Siv3d.hpp>

//Gameから各クラスに渡すやつ
struct GameData {
	std::vector<Point> stdPositions; //各プレイヤーのフィールドの基準点
	std::vector<std::vector<Rect>> nextUnitFrames;
	std::vector<Rect> stockFrames;
	Size playerRegion;
	Size fieldSize;
	std::unordered_map<int, int> gamepadMapper; //実際（ハード的な）の番号とプレイヤー番号のマッピング
};