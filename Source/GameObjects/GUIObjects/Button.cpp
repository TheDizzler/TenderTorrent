#include "Button.h"

Button::Button() {


}

Button::~Button() {
}


bool Button::load(ID3D11Device* device, const wchar_t* fontFile,
	const wchar_t * upButtonFile, const wchar_t * downButtonFile) {

	buttonFont.reset(new FontSet());
	if (!buttonFont->load(device, fontFile))
		return false;
	buttonFont->setTint(normalColor);

	buttonLabel.reset(new TextLabel(
		Vector2(0, 0), buttonFont.get()));

	normalSprite.reset(new Sprite());
	pressedSprite.reset(new Sprite());
	if (!normalSprite->load(device, upButtonFile) ||
		!pressedSprite->load(device, downButtonFile))
		return false;


	return true;
}

void Button::setPosition(Vector2& pos) {

	position = pos;

	normalSprite->setPosition(position);
	pressedSprite->setPosition(position);

	hitArea = normalSprite->getHitArea();

	buttonLabel->position = Vector2(position.x - hitArea->size.x / 3, position.y - hitArea->size.y / 3);

	
	

}

void Button::update(double deltaTime, MouseController* mouse) {

	isHover = hitArea->contains(mouse->position);

	if (isSelected && !(GetKeyState(VK_LBUTTON) & 0x8000))
		isClicked = true;
	else {
		isClicked = false;
		isSelected = isHover && GetKeyState(VK_LBUTTON) & 0x8000;

	}

}

void Button::draw(SpriteBatch* batch) {

	Sprite* drawSprite;
	drawSprite = normalSprite.get();

	if (isSelected) {
		drawSprite = pressedSprite.get();
		buttonFont->setTint(selectedColor);
	} else if (isHover)
		buttonFont->setTint(hoverColor);
	else
		buttonFont->setTint(normalColor);

	drawSprite->draw(batch);
	buttonLabel->draw(batch);

}


void Button::setText(string text) {

	buttonLabel->setText(text);
}

bool Button::clicked() {

	if (isClicked) {
		isClicked = isSelected = false;
		return true;
	}
	return false;
}
