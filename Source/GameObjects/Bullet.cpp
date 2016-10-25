#include "Bullet.h"

Bullet::Bullet() :Sprite(SimpleMath::Vector2(0, 0)) {
	isAlive = false;
}

Bullet::Bullet(const Vector2 &position) : Sprite(position) {

	hitArea.reset(new HitArea(position, Vector2(width, height)));
	isAlive = false;
}

Bullet::~Bullet() {
}

void Bullet::update(double deltaTime) {

	position.y -= bulletSpeed * deltaTime;
	timeAlive += deltaTime;

	if (position.y < 0) {
		isAlive = false;
		position = weaponStore;
		timeAlive = 0;
	}

	Sprite::update(deltaTime);
}

//void Bullet::draw(SpriteBatch* batch, Sprite* baseSprite) {
//
//	batch->Draw(baseSprite->getTexture().Get(), position,
//		&(baseSprite->getRect()), tint, rotation, baseSprite->getOrigin(),
//		scale, SpriteEffects_None, layerDepth);
//
//}


