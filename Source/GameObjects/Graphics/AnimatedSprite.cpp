#include "AnimatedSprite.h"



//AnimatedSprite::AnimatedSprite() {
//}

AnimatedSprite::AnimatedSprite(const Vector2& pos) {
	position = pos;
	hitArea.reset(new HitArea(position, Vector2(0, 0)));
}

AnimatedSprite::~AnimatedSprite() {
}

void AnimatedSprite::load(shared_ptr<Animation> anim) {

	animation = anim;

	if (animation->animationFrames.size() > 0)
		currentFrameIndex = 0;

	hitArea.reset(new HitArea(position, Vector2(getWidth(), getHeight())));
	origin = Vector2(getWidth() / 2, getHeight() / 2);
}


void AnimatedSprite::update(double deltaTime) {

	currentFrameTime += deltaTime;
	if (currentFrameTime >= animation->timePerFrame) {
		if (++currentFrameIndex >= animation->animationFrames.size())
			currentFrameIndex = -1;
		currentFrameTime = 0;
	}
}

void AnimatedSprite::draw(SpriteBatch* batch) {
	batch->Draw(animation->texture.Get(), position,
		&animation->animationFrames[currentFrameIndex]->sourceRect, tint, rotation,
		origin, scale, SpriteEffects_None, layerDepth);
}



const Vector2& AnimatedSprite::getPosition() const {
	return position;
}

const Vector2& AnimatedSprite::getOrigin() const {
	return origin;
}

const Vector2& AnimatedSprite::getScale() const {
	return scale;
}

const float AnimatedSprite::getRotation() const {
	return rotation;
}

const Color& AnimatedSprite::getTint() const {
	return tint;
}

const float AnimatedSprite::getAlpha() const {
	return tint.w;
}

const RECT AnimatedSprite::getRect() const {
	return animation->animationFrames[currentFrameIndex]->sourceRect;
}

const int AnimatedSprite::getWidth() const {
	return animation->animationFrames[currentFrameIndex]->sourceRect.right
		- animation->animationFrames[currentFrameIndex]->sourceRect.left;
}

const int AnimatedSprite::getHeight() const {
	return animation->animationFrames[currentFrameIndex]->sourceRect.bottom
		- animation->animationFrames[currentFrameIndex]->sourceRect.top;
}

const HitArea* AnimatedSprite::getHitArea() const {
	return hitArea.get();
}

void AnimatedSprite::moveBy(const Vector2& moveVector) {
	setPosition(position + moveVector);
}

void AnimatedSprite::setPosition(const Vector2& pos) {

	position = pos;
	hitArea->position = Vector2(position.x - origin.x * scale.x,
		position.y - origin.y * scale.y);
	hitArea->size = Vector2(getWidth() * scale.x, getHeight() * scale.y);
}


void AnimatedSprite::setOrigin(const Vector2& orgn) {
	origin = orgn;
}

void AnimatedSprite::setScale(const Vector2& scl) {

	scale = scl;
	hitArea->position = Vector2(position.x - origin.x*scale.x,
		position.y - origin.y*scale.y);
	hitArea->size = Vector2(getWidth() * scale.x, getHeight() * scale.y);
}

void AnimatedSprite::setRotation(const float rot) {
	rotation = rot;
}

void AnimatedSprite::setTint(const XMFLOAT4 colr) {
	tint = colr;
}

void AnimatedSprite::setAlpha(const float alpha) {
	tint.w = alpha;
}

