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

	Vector2 size = buttonFont->measureString(buttonLabel->getText());
	buttonLabel->position = Vector2(position.x - size.x / 2, position.y - size.y / 2 - 5);




}



void Button::update(double deltaTime, MouseController* mouse) {

	isHover = hitArea->contains(mouse->position);

	if (isSelected && !(GetKeyState(VK_LBUTTON) & 0x8000))
		isClicked = true;
	else {
		isClicked = false;
		if (!isHover)
			isSelected = false;
		else
			if (!lastButtonStateDown && (GetKeyState(VK_LBUTTON) & 0x8000))
				isSelected = true;

	}

	lastButtonStateDown = GetKeyState(VK_LBUTTON) & 0x8000;
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

void Button::setScale(const Vector2 & scale) {

	normalSprite->setScale(scale);
	pressedSprite->setScale(scale);

	hitArea = normalSprite->getHitArea();
}

int Button::getWidth() {


	return hitArea->size.x;
}

bool Button::clicked() {

	if (isClicked) {
		isClicked = isSelected = false;
		return true;
	}
	return false;
}
