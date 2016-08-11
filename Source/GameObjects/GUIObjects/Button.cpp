#include "Button.h"


Button::Button() {
}

Button::~Button() {
}


void Button::update(double deltaTime, MouseController* mouse) {

	isHover = hitArea->contains(mouse->getPosition());

	if (isSelected && !mouse->leftButtonDown()) {
		isClicked = true;
		//mouse->leftButtonHandled();
	} else {
		isClicked = false;
		if (!isHover)
			isSelected = false;
		//else if (!lastButtonStateDown && mouse->leftButtonDown()) {
		else if (!mouse->leftButtonLastDown() && mouse->leftButtonDown()) {
			isSelected = true;
			//mouse->leftButtonHandled();
		}

	}

	//lastButtonStateDown = mouse->leftButtonDown();
}


void Button::setPosition(Vector2& pos) {

	position = pos;

	normalSprite->setPosition(position);
	pressedSprite->setPosition(position);

	hitArea = normalSprite->getHitArea();
}

const Vector2& Button::getPosition() {
	return position;
}



void Button::setScale(const Vector2 & scale) {

	normalSprite->setScale(scale);
	pressedSprite->setScale(scale);

	hitArea = normalSprite->getHitArea();
}

int Button::getWidth() {

	return hitArea->size.x;
}

int Button::getHeight() {

	return hitArea->size.y;
}

bool Button::clicked() {

	if (isClicked) {
		isClicked = isSelected = false;
		return true;
	}
	return false;
}
