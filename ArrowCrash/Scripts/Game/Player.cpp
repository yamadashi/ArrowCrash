#include "Player.h"

Player::Player(int player_num, GameData& gameData_)
	:number(player_num),
	gameData(gameData_),
	score(0),
	penalty(-40),
	timer(true),
	arrowBlocks(new std::vector<std::weak_ptr<ArrowBlock>>()),
	field(new Field(gameData.stdPositions.at(number), *arrowBlocks)),
	mngr(new BlockUnitManager(*field, *arrowBlocks, gameData, number)),
	ojamaGauge(none)
{
	static const Size gauge_size = TextureAsset(L"gauge").size;
	double scale = (double)field->getFieldShape().h / gauge_size.y;
	ojamaGauge.emplace(field->getFieldShape().pos.movedBy(-gauge_size.x*scale*1.5, 0), scale, *mngr);
}

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
		if (gamepad.pressed(ymds::GamepadIn::DOWN)) {
			if (timer.ms() > 100) {
				mngr->getCurrentUnit().move(MoveDirection::Down);
				timer.restart();
			}
		}
		if (gamepad.clicked(ymds::GamepadIn::THREE)) mngr->getCurrentUnit().rotate(RotateDirection::Left);
		if (gamepad.clicked(ymds::GamepadIn::TWO)) mngr->getCurrentUnit().rotate(RotateDirection::Right);
		if (gamepad.clicked(ymds::GamepadIn::L1)) mngr->exchangeStock();
		if (gamepad.clicked(ymds::GamepadIn::R1)) explode();
	}

	field->update();
	if (field->deathCheck()) {
		PutText(L"", penalty).from(gameData.stdPositions.at(number) + Point(64, 450));
		if (score >= -penalty)
			score += penalty;
		else score = 0;
		field->restart();
	}
	mngr->update();
}

void Player::draw() const {
	field->draw();
	ojamaGauge->draw();
	mngr->draw();
	PutText(L"Score:", score).from(gameData.stdPositions.at(number) + Point(64, 384));
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

	//お邪魔
	mngr->bother(numOfDestroyed);

	mngr->closeField();
	mngr->getCurrentUnit().predict();
}