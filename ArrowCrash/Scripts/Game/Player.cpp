#include "Player.h"

Player::Player(int player_num, const GameData& gameData_)
	:number(player_num),
	gameData(gameData_),
	arrowBlocks(new std::vector<std::weak_ptr<ArrowBlock>>()),
	field(new Field(gameData.stdPositions.at(number), gameData.blockSize)),
	mngr(*field, *arrowBlocks, gameData, number)
{}

void Player::update() {

	if (Input::KeyUp.clicked) mngr.getCurrentUnit().fallImmediately();
	else if (Input::KeyLeft.clicked) mngr.getCurrentUnit().move(MoveDirection::Left);
	else if (Input::KeyRight.clicked) mngr.getCurrentUnit().move(MoveDirection::Right);
	else if (Input::KeyDown.clicked) mngr.getCurrentUnit().move(MoveDirection::Down);
	else if (Input::KeyA.clicked) mngr.getCurrentUnit().rotate(RotateDirection::Left);
	else if (Input::KeyD.clicked) mngr.getCurrentUnit().rotate(RotateDirection::Right);
	else if (Input::KeyS.clicked) mngr.exchangeStock();

	if (Input::KeySpace.clicked) explode();

	field->update();
	mngr.update();
}

void Player::draw() const {
	field->draw();
	mngr.draw();
}

void Player::explode() {
	for (auto arrow : *arrowBlocks) {
		const auto& ptr = arrow.lock();
		if (ptr->isSettled())
			field->explode(ptr->getPoint(), ptr->getDirection());
	}
	//ちょっと気持ち悪い
	auto&& itr = std::remove_if(arrowBlocks->begin(), arrowBlocks->end(), [](const std::weak_ptr<ArrowBlock>& ref) { return ref.lock()->isDestroyed(); });
	arrowBlocks->erase(itr, arrowBlocks->end());
}