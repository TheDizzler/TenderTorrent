#include "Tatter.h"

#include <random>
Tatter::Tatter(const Vector2& pos) : Sprite(pos) {

	mt19937 rng;
	rng.seed(random_device{}());
	uniform_int_distribution<mt19937::result_type> rand((int) 0, XM_2PI);

	direction = Vector2(cos(rand(rng)), sin(rand(rng)));
	direction.Normalize();
}

Tatter::~Tatter() {
}

void Tatter::update(double deltaTime) {

	moveBy(deltaTime*direction*speed);
}
