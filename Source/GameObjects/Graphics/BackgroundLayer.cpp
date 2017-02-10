#include "BackgroundLayer.h"

BackgroundLayer::BackgroundLayer() {
}

BackgroundLayer::~BackgroundLayer() {
}

void BackgroundLayer::load(GraphicsAsset* const graphicsAsset) {
	whole = make_unique<Sprite>();
	whole->load(graphicsAsset);
	whole->setOrigin(Vector2::Zero);
}

void BackgroundLayer::loadPiece(GraphicsAsset* const graphicsAsset) {

	unique_ptr<Sprite> newTatter = make_unique<Sprite>(whole->getPosition());
	newTatter->load(graphicsAsset);
	newTatter->setOrigin(Vector2::Zero);

	tatters.push_back(move(newTatter));
}

void BackgroundLayer::setHealth(int hlth) {

	health = hlth;
}

#include "../../Managers/GameManager.h"
void BackgroundLayer::setInitialPosition(const Vector2& pos, const Vector2& scl) {

	scale = scl;
	whole->setScale(scale);
	whole->setPosition(pos);

	projectedHitArea = make_unique<HitArea>(whole->getHitArea()->position, whole->getHitArea()->size);

	Vector2 position = whole->getPosition();
	Vector2 textPos(position.x + whole->getWidth() * 3 / 4, position.y + whole->getHeight() * 3 / 4);
	healthLabel.reset(guiFactory->createTextLabel(textPos, to_wstring(health)));
}




//void BackgroundLayer::setHitArea(const Vector2& hitPos, const Vector2& size) {
//
//	Vector2 offset((position.x - width * scale.x / 2) + hitPos.x * scale.x,
//		(position.y - height* scale.y / 2) + hitPos.y* scale.y);
//	hitArea->position = offset;
//	hitArea->size = size * scale;
//
//	Vector2 textPos(offset.x + hitArea->size.x * 3 / 4, offset.y + hitArea->size.y * 3 / 4);
//	//healthLabel.reset(new TextLabel(textPos, font));
//	//healthLabel->setText(to_string(health));
//	healthLabel.reset(guiFactory->createTextLabel(textPos, to_wstring(health)));
//
//
//	topLeftCornerPos = hitArea->position;
//	topRightCornerPos = Vector2(hitArea->position.x + hitArea->size.x, hitArea->position.y);
//	bottomLeftCornerPos = Vector2(hitArea->position.x, hitArea->position.y + hitArea->size.y);
//	bottomRightCornerPos = Vector2(hitArea->position.x + hitArea->size.x, hitArea->position.y + hitArea->size.y);
//
//	projectedHitArea = make_unique<HitArea>(hitArea->position, hitArea->size);
//	updateProjectedHitArea();
//}



void BackgroundLayer::takeDamage(int damageTaken) {

	health -= damageTaken;
	labelHidden = false;
	healthLabel->setText(to_wstring(health));
	if (health <= 0) {
		whole->isAlive = false;
	}
}

void BackgroundLayer::setLayerDepth(float layerDepth) {

}

void BackgroundLayer::moveBy(const Vector2& pos) {

	whole->moveBy(pos);
	//hitArea->position += pos;
	updateProjectedHitArea();
	healthLabel->moveBy(pos);
	topLeftCornerPos += pos;
	topRightCornerPos += pos;
	bottomLeftCornerPos += pos;
	bottomRightCornerPos += pos;

}

void BackgroundLayer::draw(SpriteBatch* batch, Sprite* frame) {

	if (whole->isAlive) {
		whole->draw(batch);

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

const HitArea* BackgroundLayer::getHitArea() const {
	return projectedHitArea.get();
}


void BackgroundLayer::updateProjectedHitArea() {

	Vector2 screenCords = getScreenPosition(translationMatrix());
	projectedHitArea->position = screenCords;
	projectedHitArea->size = whole->getHitArea()->size;
}

const Vector2& BackgroundLayer::getScreenPosition(Matrix viewProjectionMatrix) const {

	Vector2 screenCords = XMVector2Transform(whole->getHitArea()->position, viewProjectionMatrix);
	return screenCords;

}

unique_ptr<HitArea> BackgroundLayer::getScreenHitArea(Matrix viewProjectionMatrix) const {

	Vector2 screenCords = getScreenPosition(viewProjectionMatrix);
	unique_ptr<HitArea> projectedHitArea;
	projectedHitArea.reset(new HitArea(screenCords, whole->getHitArea()->size));
	return projectedHitArea;
}

