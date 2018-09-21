#pragma once


struct GameData {
	int cellSize; //ブロックのサイズ
	std::vector<Point> stdPositions; //各プレイヤーのフィールドの基準点
	int topUIMargin;
	int topMargin, bottomMargin;
};