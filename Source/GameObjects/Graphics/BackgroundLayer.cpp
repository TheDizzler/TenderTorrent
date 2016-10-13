#include "BackgroundLayer.h"

BackgroundLayer::BackgroundLayer() {
}

BackgroundLayer::~BackgroundLayer() {
}


void BackgroundLayer::setInitialPosition(const Vector2& pos, const Vector2& scl) {

	scale = scl;
	position = pos;
}

#include "../../Managers/GameManager.h"
void BackgroundLayer::setHitArea(const Vector2& hitPos, const Vector2& size) {

	Vector2 offset((position.x - width * scale.x / 2) + hitPos.x * scale.x,
		(position.y - height* scale.y / 2) + hitPos.y* scale.y);
	hitArea->position = offset;
	hitArea->size = size * scale;

	Vector2 textPos(offset.x + hitArea->size.x * 3 / 4, offset.y + hitArea->size.y * 3 / 4);
	//healthLabel.reset(new TextLabel(textPos, font));
	//healthLabel->setText(to_string(health));
	healthLabel.reset(GameManager::guiFactory->createTextLabel(textPos, to_wstring(health)));
	


	topLeftCornerPos = hitArea->position;
	topRightCornerPos = Vector2(hitArea->position.x + hitArea->size.x, hitArea->position.y);
	bottomLeftCornerPos = Vector2(hitArea->position.x, hitArea->position.y + hitArea->size.y);
	bottomRightCornerPos = Vector2(hitArea->position.x + hitArea->size.x, hitArea->position.y + hitArea->size.y);
}



void BackgroundLayer::takeDamage(int damageTaken) {

	health -= damageTaken;
	labelHidden = false;
	healthLabel->setText(to_wstring(health));
	if (health <= 0) {
		isAlive = false;
	}
}

void BackgroundLayer::setPosition(const Vector2& pos) {

	position += pos;
	hitArea->position += pos;
	healthLabel->moveBy(pos);
	topLeftCornerPos += pos;
	topRightCornerPos += pos;
	bottomLeftCornerPos += pos;
	bottomRightCornerPos += pos;
}

void BackgroundLayer::draw(SpriteBatch* batch, Sprite* frame) {

	if (isAlive) {
		Sprite::draw(batch);
		if (!labelHidden) {
			healthLabel->draw(batch);
			ID3D11ShaderResourceView* texture = frame->getTexture().Get();
			RECT frameRect = frame->getRect();
			Color frameTint = frame->getTint();
			Vector2 frameOrigin = frame->getOrigin();
			batch->Draw(texture, topLeftCornerPos, &frameRect,
				frameTint, 0, frameOrigin, scale, SpriteEffects_None, layerDepth);
			batch->Draw(texture, topRightCornerPos, &frameRect,
				frameTint, XM_PI / 2, frameOrigin, scale, SpriteEffects_None, layerDepth);
			batch->Draw(texture, bottomLeftCornerPos, &frameRect,
				frameTint, -XM_PI / 2, frameOrigin, scale, SpriteEffects_None, layerDepth);
			batch->Draw(texture, bottomRightCornerPos, &frameRect,
				frameTint, XM_PI, frameOrigin, scale, SpriteEffects_None, layerDepth);

		}
	}
}

