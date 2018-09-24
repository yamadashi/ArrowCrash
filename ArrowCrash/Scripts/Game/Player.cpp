#include "Player.h"

Player::Player(int player_num, const GameData& gameData_)
	:number(player_num),
	gameData(gameData_),
	field(gameData.stdPositions.at(number),gameData.blockSize),
	generator(field, arrowBlocks)
{}

void Player::update() {
	generator.update();
}

void Player::draw() const {
	field.draw();
}