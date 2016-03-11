#pragma once

#include "MouseController.h"
#include "TextLabel.h"
#include "../../globals.h"


static enum Actions { EXIT, PLAY };

/** A visual and logical representation of a button.
	Provides no actions on click; that must be handled else where.*/
class Button {
public:
	Button();
	~Button();

	bool load(ID3D11Device* device, const wchar_t* fontFile,
		const wchar_t* upButtonFile, const wchar_t* downButtonFile);
	virtual void setPosition(Vector2& position);
	void setText(string text);


	void update(double deltaTime, MouseController* mouse);
	void draw(SpriteBatch* batch);



	Color normalColor = Color((Vector3(1, 1, 1)));
	Color hoverColor = Color((Vector3(.5, .75, 1)));;
	Color selectedColor = Color((Vector3(0, .5, 1)));;

	bool clicked();

	Actions action;
private:

	Vector2 position;
	const HitArea* hitArea;

	unique_ptr<Sprite> normalSprite;
	unique_ptr<Sprite> pressedSprite;

		/** Button is held down over button but has not been clicked */
	bool isSelected = false;
	bool isHover = false;
	bool isClicked = false;

	unique_ptr<FontSet> buttonFont;
	unique_ptr<TextLabel> buttonLabel;
};