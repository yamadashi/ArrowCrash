#pragma once
#include <Siv3d.hpp>

struct GameData {
	int cellSize; //�u���b�N�̃T�C�Y
	std::vector<Point> stdPositions; //�e�v���C���[�̃t�B�[���h�̊�_
	int topUIMargin;
	int topMargin, bottomMargin;
};