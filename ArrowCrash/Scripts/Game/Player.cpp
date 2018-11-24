#include "Player.h"

Player::Player(int player_num, GameData& gameData_)
	:number(player_num),
	gameData(gameData_),
	score(0),
	penalty(500),
	timer(true),
	arrowBlocks(new std::vector<std::weak_ptr<ArrowBlock>>()),
	field(new Field(gameData.stdPositions.at(number), *arrowBlocks)),
	mngr(new BlockUnitManager(*field, *arrowBlocks, gameData, number)),
	ojamaGauge(none),
	infoWindow(none)
{
	static const Size gauge_size = TextureAsset(L"gauge").size;
	double scale = (double)field->getFieldShape().h / gauge_size.y;
	ojamaGauge.emplace(field->getFieldShape().pos.movedBy(-gauge_size.x*scale*1.5, 0), scale, *mngr);
}

std::vector < Player *> Player::players;

void Player::update() {

	auto& gamepad = ymds::GamepadManager::get().getGamepad(number);

	if (gamepad.clicked(ymds::GamepadIn::UP)) mngr->getCurrentUnit().fallImmediately();

	else {
		if (gamepad.pressed(ymds::GamepadIn::LEFT)) {
			if (timer.ms() > 100) {
				mngr->getCurrentUnit().move(MoveDirection::Left);
				timer.restart();
			}
		}
		else if (gamepad.pressed(ymds::GamepadIn::RIGHT)) {
			if (timer.ms() > 100) {
				mngr->getCurrentUnit().move(MoveDirection::Right);
				timer.restart();
			}
		}
		else if (gamepad.pressed(ymds::GamepadIn::DOWN)) {
			if (timer.ms() > 100) {
				mngr->getCurrentUnit().move(MoveDirection::Down);
				timer.restart();
			}
		}		
		else if (gamepad.clicked(ymds::GamepadIn::L1)) mngr->exchangeStock();
		else if (gamepad.pressed(ymds::GamepadIn::R1)) explode();
		if (gamepad.clicked(ymds::GamepadIn::THREE)) mngr->getCurrentUnit().rotate(RotateDirection::Left);
		else if (gamepad.clicked(ymds::GamepadIn::TWO)) mngr->getCurrentUnit().rotate(RotateDirection::Right);
	}


	mngr->update();
}

void Player::draw() const {
	field->draw();
	ojamaGauge->draw();
	infoWindow->draw();
	mngr->draw();
}

void Player::explode() {
	int numOfDestroyed = 0;
	static bool explosion_flag = false;
	for (auto arrow : *arrowBlocks) {
		const auto& ptr = arrow.lock();
		if (ptr->isSettled()) {
			if (!explosion_flag) {
				explosion_flag = true;
				SoundAsset(L"explosion").playMulti();
			}
			numOfDestroyed += ptr->explode();
		}
	}
	explosion_flag = false;
	//ちょっと気持ち悪い
	auto&& itr = std::remove_if(arrowBlocks->begin(), arrowBlocks->end(), [](const std::weak_ptr<ArrowBlock>& ref) { return ref.lock()->isDestroyed(); });
	arrowBlocks->erase(itr, arrowBlocks->end());

	score += 10 * int(pow(numOfDestroyed, 1.2));

	//お邪魔
	mngr->bother(numOfDestroyed);

	mngr->closeField();
	mngr->getCurrentUnit().predict();
}