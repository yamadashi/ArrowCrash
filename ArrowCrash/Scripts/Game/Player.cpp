#include "Player.h"

Player::Player(int player_num, const GameData& gameData_)
	:number(player_num),
	gameData(gameData_),
	counter(0),
	field(new Field(gameData.stdPositions.at(number), gameData.blockSize)),
	mngr(*field, arrowBlocks, gameData.stdPositions.at(number), gameData.blockSize)
{}

void Player::update() {	

	if (Input::KeyUp.clicked) mngr.getCurrentUnit().fallImmediately();
	else if (Input::KeyLeft.pressed) {
		counter++;
		if (counter % 10 == 1)	mngr.getCurrentUnit().move(MoveDirection::Left);
	}
	else if (Input::KeyLeft.released) counter = 0;
	else if (Input::KeyRight.pressed) {
		counter++;
		if (counter % 10 == 1)	mngr.getCurrentUnit().move(MoveDirection::Right);
	}
	else if (Input::KeyRight.released) counter = 0;	
	else if (Input::KeyDown.pressed){
		counter++;
		if (counter % 8 == 1)	mngr.getCurrentUnit().move(MoveDirection::Down);
	}
	else if (Input::KeyDown.released) counter = 0;
	else if (Input::KeyA.clicked) mngr.getCurrentUnit().rotate(RotateDirection::Left);
	else if (Input::KeyD.clicked) mngr.getCurrentUnit().rotate(RotateDirection::Right);

	if (Input::KeySpace.clicked) explode();

	field->update();
	mngr.update();
}

void Player::draw() const {
	field->draw();
	mngr.draw();
}

void Player::explode() {
	for (auto&& arrow : arrowBlocks) {
		const auto& ptr = arrow.lock();
		if (ptr->isSettled())
			field->explode(ptr->getPoint(), ptr->getDirection());
	}
	//‚¿‚å‚Á‚Æ‹C‚¿ˆ«‚¢‚Ì‚Å‘‚«Š·‚¦‚½‚¢
	std::remove_if(arrowBlocks.begin(), arrowBlocks.end(), [](const std::weak_ptr<ArrowBlock>& ref) { return ref.lock()->isSettled(); });
}