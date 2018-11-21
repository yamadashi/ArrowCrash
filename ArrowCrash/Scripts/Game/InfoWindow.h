#pragma once
#include <Siv3D.hpp>
#include "./GameData.h"

class InfoWindow {
private:
	static std::unique_ptr<Font> font;
	int playerNumber;
	const int& score;
	const Size size;
	const Point centerPos;

public:
	InfoWindow(int playerNum, GameData& data, const int& score_);
	~InfoWindow() = default;

	void draw() const;

	static void setFont(String handler, int size);
};