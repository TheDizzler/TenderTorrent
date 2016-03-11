#include "Button.h"

Button::Button(ID3D11Device * device, const wchar_t* file) {

	buttonFont.reset(new FontSet());
	buttonFont->load(device, file);
	buttonFont->layerDepth = .1f;
	buttonFont->setTint(normalColor);

	buttonLabel.reset(new TextLabel(
		Vector2(0, 0), buttonFont.get()));
}

Button::~Button() {
}


void Button::setPosition(Vector2& pos) {

	position = pos;
	buttonLabel->position = Vector2(position.x - width / 3, position.y - height / 3);

	hitArea->position = position;
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

	Sprite::draw(batch);

	if (isSelected)
		buttonFont->setTint(selectedColor);
	else if (isHover)
		buttonFont->setTint(hoverColor);
	else
		buttonFont->setTint(normalColor);

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
