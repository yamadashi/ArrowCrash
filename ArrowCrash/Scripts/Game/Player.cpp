#include "Player.h"

Player::Player(int player_num, const GameData& gameData_)
	:number(player_num),
	gameData(gameData_),
	arrowBlocks(),
	field(gameData.stdPositions.at(number),gameData.blockSize),
	generator(field, arrowBlocks)
{}

void Player::update() {
	generator.update();
	if (Input::KeySpace.clicked) explode();

	field.update();
}

void Player::draw() const {
	field.draw();
}

void Player::explode() {
	for (auto&& arrow : arrowBlocks) {
		const auto& ptr = arrow.lock();
		if (ptr->isSettled())
			field.explode(ptr->getPoint(), ptr->getDirection());
	}
	//‚¿‚å‚Á‚Æ‹C‚¿ˆ«‚¢‚Ì‚Å‘‚«Š·‚¦‚½‚¢
	std::remove_if(arrowBlocks.begin(), arrowBlocks.end(), [](const std::weak_ptr<ArrowBlock>& ref) { return ref.lock()->isSettled(); });
}