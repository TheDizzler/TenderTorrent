#include "BackgroundLayer.h"

BackgroundLayer::BackgroundLayer() {
}

BackgroundLayer::~BackgroundLayer() {
}


void BackgroundLayer::setInitialPosition(const Vector2& pos, const Vector2& scl) {

	scale = scl;
	position = pos;
}

void BackgroundLayer::setHitArea(const Vector2& hitPos, const Vector2& size, FontSet* font) {

	Vector2 offset((position.x - width * scale.x / 2) + hitPos.x * scale.x,
		(position.y - height* scale.y / 2) + hitPos.y* scale.y);
	hitArea->position = offset;
	hitArea->size = size * scale;

	Vector2 textPos(offset.x + hitArea->size.x * 3 / 4, offset.y + hitArea->size.y * 3 / 4);
	healthLabel.reset(new TextLabel(textPos, font));
	healthLabel->setText(to_string(health));
}



void BackgroundLayer::takeDamage(int damageTaken) {

	health -= damageTaken;
	labelHidden  = false;
	healthLabel->setText(to_string(health));
	if (health <= 0) {
		isAlive = false;
	}
}

void BackgroundLayer::setPosition(const Vector2& pos) {

	position += pos;
	hitArea->position += pos;
	healthLabel->position += pos;
}

void BackgroundLayer::draw(SpriteBatch * batch) {

	if (isAlive) {
		Sprite::draw(batch);
		if (!labelHidden)
			healthLabel->draw(batch);
	}
}

