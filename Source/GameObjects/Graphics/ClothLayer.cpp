#include "../../pch.h"
#include "ClothLayer.h"
#include "../../Managers/GameManager.h"


ClothLayer::~ClothLayer() {
}

void ClothLayer::load(GraphicsAsset* const graphicsAsset, Sprite* cornerFrame) {

	//wholeCloth = make_unique<GameObject>();
	wholeCloth.load(graphicsAsset);
	wholeCloth.setOrigin(Vector2::Zero);

	//frame = cornerFrame;
	frameTexture = cornerFrame->getTexture();
	frameRect = cornerFrame->getRect();
	frameTint = cornerFrame->getTint();
	frameOrigin = cornerFrame->getOrigin();
	frameLayerDepth = cornerFrame->getLayerDepth();
}

void ClothLayer::loadPiece(GraphicsAsset* const graphicsAsset) {

	unique_ptr<Tatter> newTatter = make_unique<Tatter>(wholeCloth.getPosition());
	newTatter->load(graphicsAsset);
	newTatter->setOrigin(Vector2::Zero);

	tatters.push_back(move(newTatter));
}

void ClothLayer::reloadGraphicsAssets() {
	wholeCloth.reloadGraphicsAsset(&guiFactory);

	healthLabel->reloadGraphicsAsset();
}

void ClothLayer::setHealth(int hlth) {

	health = hlth;
}

void ClothLayer::setInitialPosition(const Vector2& pos, const Vector2& scl) {

	scale = scl;
	wholeCloth.setScale(scale);
	wholeCloth.setPosition(pos);

	Vector2 position = wholeCloth.getPosition();
	Vector2 textPos(position.x + wholeCloth.getWidth() * 3 / 4,
		position.y + wholeCloth.getHeight() * 3 / 4);
	healthLabel.reset(guiFactory.createTextLabel(textPos, to_wstring(health)));
	healthLabel->setTint(Color(0, 0, 0, 1));

	hitArea = &wholeCloth.getHitArea();
	topLeftCornerPos = hitArea.position;
	topRightCornerPos = Vector2(hitArea.position.x + hitArea.size.x, hitArea.position.y);
	bottomLeftCornerPos = Vector2(hitArea.position.x,
		hitArea.position.y + hitArea.size.y);
	bottomRightCornerPos = Vector2(hitArea.position.x + hitArea.size.x,
		hitArea.position.y + hitArea.size.y);

}




//void ClothLayer::setHitArea(const Vector2& hitPos, const Vector2& size) {
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



void ClothLayer::takeDamage(int damageTaken) {

	health -= damageTaken;
	labelHidden = false;
	healthLabel->setText(to_wstring(health));
	if (health <= 0) {
		wholeCloth.isAlive = false;
		wholeCloth.isExploding = true;
		//for (const auto& piece : tatters)
		//	piece->setDirection();
	}
}

void ClothLayer::setLayerDepth(float depth) {
	wholeCloth.setLayerDepth(depth);
	frameLayerDepth = depth + .1f;
}

void ClothLayer::moveBy(const Vector2& pos) {

	wholeCloth.moveBy(pos);
	//hitArea->position += pos;
	//updateProjectedHitArea();
	healthLabel->moveBy(pos);
	topLeftCornerPos += pos;
	topRightCornerPos += pos;
	bottomLeftCornerPos += pos;
	bottomRightCornerPos += pos;

}


void ClothLayer::update(double deltaTime) {

	//updateProjectedHitArea();


	if (wholeCloth.isExploding) {
		timeExploding += deltaTime;
		if (timeExploding >= EXPLODE_TIME)
			wholeCloth.isExploding = false;
		for (const auto& piece : tatters)
			piece->update(deltaTime);
	}

	healthLabel->update(deltaTime);

}

void ClothLayer::draw(SpriteBatch* batch/*, Sprite* frame*/) {

	if (wholeCloth.isAlive) {
		wholeCloth.draw(batch);

		if (!labelHidden) {
			healthLabel->draw(batch);
			//ID3D11ShaderResourceView* texture = frame->getTexture().Get();

			//frame->setPosition(wholeCloth->getPosition());
			//frame->draw(batch);
			batch->Draw(frameTexture.Get(), topLeftCornerPos, &frameRect,
				frameTint, 0, frameOrigin, scale, SpriteEffects_None, frameLayerDepth);
			batch->Draw(frameTexture.Get(), topRightCornerPos, &frameRect,
				frameTint, XM_PI / 2, frameOrigin, scale, SpriteEffects_None, frameLayerDepth);
			batch->Draw(frameTexture.Get(), bottomLeftCornerPos, &frameRect,
				frameTint, -XM_PI / 2, frameOrigin, scale, SpriteEffects_None, frameLayerDepth);
			batch->Draw(frameTexture.Get(), bottomRightCornerPos, &frameRect,
				frameTint, XM_PI, frameOrigin, scale, SpriteEffects_None, frameLayerDepth);

		}
	} else if (wholeCloth.isExploding) {
		for (const auto& piece : tatters)
			piece->draw(batch);
	}
}

const HitArea& ClothLayer::getHitArea() const {
	return hitArea;
}
//
//
//void ClothLayer::updateProjectedHitArea() {
//
//	Vector2 screenCords = getScreenPosition(translationMatrix());
//	projectedHitArea->position = screenCords;
//	projectedHitArea->size = wholeCloth.getHitArea().size * cameraZoom();
//
//	/*wostringstream wss;
//	wss << "size: " << wholeCloth->getHitArea()->size.x << ", " << wholeCloth->getHitArea()->size.y << "\n";
//	wss << "pro size: " << projectedHitArea->size.x << ", " << projectedHitArea->size.y << "\n";
//	wss << "pos: " << projectedHitArea->position.x << ", " << projectedHitArea->position.y;
//	healthLabel->setText(wss);*/
//}
//
//const Vector2 ClothLayer::getScreenPosition(Matrix viewProjectionMatrix) const {
//
//	Vector2 screenCords = XMVector2Transform(
//		wholeCloth.getHitArea().position, viewProjectionMatrix);
//	return screenCords;
//
//}
//
//unique_ptr<HitArea> ClothLayer::getScreenHitArea(Matrix viewProjectionMatrix) const {
//
//	Vector2 screenCords = getScreenPosition(viewProjectionMatrix);
//	unique_ptr<HitArea> projectedHitArea;
//	projectedHitArea.reset(new HitArea(screenCords, wholeCloth.getHitArea().size));
//	return projectedHitArea;
//}

bool ClothLayer::isAlive() {
	return wholeCloth.isAlive;
}

