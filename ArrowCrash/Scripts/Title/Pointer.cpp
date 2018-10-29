#include "Pointer.h"


const int Pointer::radius = 15;
const double Pointer::accel = 2.0;
const double Pointer::frictionCoeff = 1.0;
const double Pointer::maxSpeed = 10.0;

Pointer::Pointer(int player_num_)
	:pos(Window::Center()),
	velocity(0, 0),
	player_num(player_num_),
	color(constants::playerColor[player_num])
{}


void Pointer::move() {

	auto& gamepad = ymds::GamepadManager::get().getGamepad(player_num);
	//入力ベクトルを正規化
	Vec2 input(
		(Input::KeyRight.pressed - Input::KeyLeft.pressed),
		(Input::KeyDown.pressed - Input::KeyUp.pressed)
	);
	/*Vec2 input(
		(gamepad.pressed(ymds::GamepadIn::RIGHT) - gamepad.pressed(ymds::GamepadIn::LEFT)),
		(gamepad.pressed(ymds::GamepadIn::DOWN) - gamepad.pressed(ymds::GamepadIn::UP))
	);*/
	if (!input.isZero()) input.normalize();
	
	//加速・減速
	velocity += input * accel - (velocity.isZero() ? Vec2::Zero : velocity.normalized()) * frictionCoeff;
	
	//速度の上限・下限
	double speed = velocity.length();
	if (speed < frictionCoeff) velocity.set(0, 0);
	else if (speed > maxSpeed) {
		velocity -= (speed - maxSpeed) * velocity.normalized();
	}

	pos.moveBy(velocity);
}

bool Pointer::isOutOfRegion() const {
	return !Window::ClientRect().contains(pos);
}

bool Pointer::isClicked() const {
	return Input::KeySpace.clicked;
	//return ymds::GamepadManager::get().getGamepad(player_num).clicked(ymds::GamepadIn::TWO);
}

Point Pointer::getPos() const {
	return pos.asPoint();
}

void Pointer::update() {
	move();
	while (isOutOfRegion()) {
		pos.moveBy(-velocity.normalized());
	}
}


void Pointer::draw() const {
	Circle(pos, radius).drawFrame(2.0, 0.5, color);
	Circle(pos, radius / 5).draw(color);
}