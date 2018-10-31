#include "Player.h"

Player::Player(int player_num, const GameData& gameData_)
	:number(player_num),
	gameData(gameData_),
	score(0),
	arrowBlocks(new std::vector<std::weak_ptr<ArrowBlock>>()),
	field(new Field(gameData.stdPositions.at(number), *arrowBlocks)),
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

	/*auto& gamepad = ymds::GamepadManager::get().getGamepad(number);

	if (gamepad.clicked(ymds::GamepadIn::UP)) mngr.getCurrentUnit().fallImmediately();
	else if (gamepad.clicked(ymds::GamepadIn::LEFT)) mngr.getCurrentUnit().move(MoveDirection::Left);
	else if (gamepad.clicked(ymds::GamepadIn::RIGHT)) mngr.getCurrentUnit().move(MoveDirection::Right);
	else if (gamepad.clicked(ymds::GamepadIn::DOWN)) mngr.getCurrentUnit().move(MoveDirection::Down);
	else if (gamepad.clicked(ymds::GamepadIn::THREE)) mngr.getCurrentUnit().rotate(RotateDirection::Left);
	else if (gamepad.clicked(ymds::GamepadIn::TWO)) mngr.getCurrentUnit().rotate(RotateDirection::Right);
	
	if (gamepad.clicked(ymds::GamepadIn::L1)) mngr.exchangeStock();

	if (gamepad.clicked(ymds::GamepadIn::R1)) explode();*/
	

	field->update();
	mngr.update();
}

void Player::draw() const {
	field->draw();
	mngr.draw();
}

void Player::explode() {
	int numOfDestroyed = 0;
	for (auto arrow : *arrowBlocks) {
		const auto& ptr = arrow.lock();
		if (ptr->isSettled())
			numOfDestroyed += ptr->explode();
	}
	//ちょっと気持ち悪い
	auto&& itr = std::remove_if(arrowBlocks->begin(), arrowBlocks->end(), [](const std::weak_ptr<ArrowBlock>& ref) { return ref.lock()->isDestroyed(); });
	arrowBlocks->erase(itr, arrowBlocks->end());

	score += numOfDestroyed;
}