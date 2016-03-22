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


	topLeftCorner = hitArea->position;
	topRightCorner = Vector2(hitArea->position.x + hitArea->size.x, hitArea->position.y);
	bottomLeftCorner = Vector2(hitArea->position.x, hitArea->position.y + hitArea->size.y);
	bottomRightCorner = Vector2(hitArea->position.x + hitArea->size.x, hitArea->position.y + hitArea->size.y);
}



void BackgroundLayer::takeDamage(int damageTaken) {

	health -= damageTaken;
	labelHidden = false;
	healthLabel->setText(to_string(health));
	if (health <= 0) {
		isAlive = false;
	}
}

void BackgroundLayer::setPosition(const Vector2& pos) {

	position += pos;
	hitArea->position += pos;
	healthLabel->position += pos;
	topLeftCorner += pos;
	topRightCorner += pos;
	bottomLeftCorner += pos;
	bottomRightCorner += pos;
}

void BackgroundLayer::draw(SpriteBatch * batch, Sprite* frame) {

	if (isAlive) {
		Sprite::draw(batch);
		if (!labelHidden) {
			healthLabel->draw(batch);
			batch->Draw(frame->texture, topLeftCorner, &frame->sourceRect,
				frame->tint, 0, frame->origin, scale, SpriteEffects_None, layerDepth);
			batch->Draw(frame->texture, topRightCorner, &frame->sourceRect,
				frame->tint, XM_PI / 2, frame->origin, scale, SpriteEffects_None, layerDepth);
			batch->Draw(frame->texture, bottomLeftCorner, &frame->sourceRect,
				frame->tint, -XM_PI / 2, frame->origin, scale, SpriteEffects_None, layerDepth);
			batch->Draw(frame->texture, bottomRightCorner, &frame->sourceRect,
				frame->tint, XM_PI, frame->origin, scale, SpriteEffects_None, layerDepth);

		}
	}
}

