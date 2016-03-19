#include "BackgroundLayer.h"

BackgroundLayer::BackgroundLayer() {
}

BackgroundLayer::~BackgroundLayer() {
}

void BackgroundLayer::setHitArea(const Vector2& hitPos, const Vector2& size) {

	Vector2 offset((position.x - width / 2) + hitPos.x, (position.y - height / 2) + hitPos.y);
	hitArea->position = offset;
	hitArea->size = size;
}

void BackgroundLayer::takeDamage(int damageTaken) {

	health -= damageTaken;

	if (health <= 0) {
		isAlive = false;
	}
}

void BackgroundLayer::setPosition(const Vector2& pos) {

	position += pos;
	hitArea->position += pos;
}
