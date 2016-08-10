#pragma once

#include "MouseController.h"

using namespace std;


static enum ButtonAction {
	EXIT, PLAY, SETTINGS, CANCEL_BUTTON, OK, UP, DOWN
};

/** A visual and logical representation of a button.
Provides no actions on click; that must be handled else where.*/

class Button {
public:
	Button();
	~Button();

	void update(double deltaTime, MouseController* mouse);
	virtual void draw(SpriteBatch* batch) = 0;

	virtual void setPosition(Vector2& position);
	const Vector2& getPosition();

	void setScale(const Vector2& scale);
	int getWidth();
	int getHeight();

	Color normalColor = Color((Vector3(1, 1, 1)));
	Color hoverColor = Color((Vector3(.5, .75, 1)));;
	Color selectedColor = Color((Vector3(0, .5, 1)));;

	bool clicked();

	ButtonAction action;

protected:

	bool lastButtonStateDown;

	Vector2 position;
	const HitArea* hitArea;

	unique_ptr<Sprite> normalSprite;
	unique_ptr<Sprite> pressedSprite;

	/** Button is held down over button but has not been clicked */
	bool isSelected = false;
	bool isHover = false;
	bool isClicked = false;

};