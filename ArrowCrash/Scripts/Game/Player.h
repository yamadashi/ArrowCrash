#pragma once
#include "Field.h"
#include "BlockGenerator.h"

class Player {
private:
	const int number; //�v���C���[�ԍ�
	const GameData& gameData;

	Field field;
	// �ꊇ�Ŕ������邽�߃t�B�[���h��̑S�Ă�ArrowBlock�ւ̎Q�Ƃ������Ă���
	std::vector<std::weak_ptr<ArrowBlock>> arrowBlocks;
	BlockGenerator generator;

public:
	Player(int player_num ,const GameData& gameData);
	~Player() = default;

	void update();
	void draw() const;
};