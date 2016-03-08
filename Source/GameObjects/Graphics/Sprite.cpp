#include "Sprite.h"
#include "DDSTextureLoader.h"
#include "../../globals.h"


Sprite::Sprite() {

	alpha = 1.0f;
	rotation = 0.0f;
	scale = Vector2(1, 1);
	tint = DirectX::Colors::White.v;
	layerDepth = 0.0f;
}

Sprite::Sprite(const Vector2& pos) {

	position = pos;
	alpha = 1.0f;
	rotation = 0.0f;
	scale = Vector2(1, 1);
	tint = DirectX::Colors::White.v;
	layerDepth = 0.0f;

}

Sprite::~Sprite() {

	if (texture)
		texture->Release();
	if (resource)
		resource->Release();
}


bool Sprite::load(ID3D11Device* device, const wchar_t* textureFile) {


	if (Globals::reportError(CreateDDSTextureFromFile(device, textureFile, &resource, &texture))) {
		MessageBox(NULL, L"Failed to load sprite", L"ERROR", MB_OK);
		return false;
	}

	Globals::getTextureDimensions(resource, &width, &height);
	origin = Vector2(width / 2.0f, height / 2.0f);
	sourceRect.left = 0;
	sourceRect.top = 0;
	sourceRect.bottom = height;
	sourceRect.right = width;

	hitArea = new HitArea(Vector2(position.x - width / 2, position.y - height / 2),
		Vector2(width - 2, height - 2));


	return true;
}


void Sprite::draw(SpriteBatch* batch) {

	batch->Draw(texture, position, &sourceRect, tint, rotation, origin, scale, SpriteEffects_None, layerDepth);

}


void Sprite::update(double deltaTime) {

	hitArea->position = position;
}


const HitArea* Sprite::getHitArea() const {
	return hitArea;
}

const Vector2& Sprite::getPosition() const {

	return position;
}


const Vector2& Sprite::getOrigin() const {

	return origin;
}

const Vector2& Sprite::getScale() const {

	return scale;
}

const float Sprite::getRotation() const {
	return rotation;
}


const Color& Sprite::getTint() const {

	return tint;
}


const float Sprite::getAlpha() const {

	return alpha;
}

const RECT Sprite::getRect() const {
	return sourceRect;
}


void Sprite::setHitArea(const HitArea* hitarea) {

	hitArea = new HitArea(Vector2(position.x - width / 2, position.y - height / 2),
		Vector2(hitarea->size.x - 2, hitarea->size.y - 2));
}

void Sprite::setPostion(const Vector2& pos) {

	position = pos;
}


void Sprite::setOrigin(const Vector2& origin) {


}


void Sprite::setScale(const Vector2& scale) {


}


void Sprite::setRotation(const float rotation) {

}


void Sprite::setTint(const Color& color) {

}


void Sprite::setAlpha(const float alpha) {

}


