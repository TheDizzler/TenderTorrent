#pragma once

#include <vector>

#include "TextLabel.h"
#include "ImageButton.h"
#include "../Graphics/RectangleSprite.h"

using namespace std;

class ListItem : public TextLabel {
public:

	ListItem(/*Vector2 pos, */const int width, const int height,
		FontSet* fnt, ID3D11ShaderResourceView* pixelTexture);
	~ListItem();


	bool update(double deltaTime, MouseController* mouse);
	void updatePosition(const Vector2& position);
	/*virtual */void draw(SpriteBatch* batch);


	bool isSelected = false;

private:

	RECT itemRect;
	unique_ptr<HitArea> hitArea;

	/** itemPosition: position of entire Item.
			position: for actual text only. */
	Vector2 itemPosition;

	size_t textMarginX = 10;
	size_t textMarginY = 5;

	bool isHover = false;
	bool buttonDownLast = false;

	ID3D11ShaderResourceView* pixel;

};

class Scrubber : public RectangleSprite {
public:
	Scrubber(ID3D11ShaderResourceView* pixel);
	~Scrubber();

	virtual void setDimensions(const Vector2& startPosition,
		const Vector2& size, const int scrollBarHeight);
	void setSize(const Vector2& size);

	virtual void update(double deltaTime, MouseController* mouse);
	//virtual void draw(SpriteBatch* batch);

	bool pressed();


	Color normalColor = Color((Vector3(1, 1, 0)));
	Color hoverColor = Color((Vector3(.5, .75, 1)));;
	Color selectedColor = Color((Vector3(0, .5, 1)));;

	float percentAt = 0;

private:

	Vector2 minPosition;
	Vector2 maxPosition;


	int scrollBarHeight;

	bool isHover = false;
	bool isPressed = false;

	int pressedPosition;
	/* Difference between max position and min position. */
	float minMaxDifference;
};


class ScrollBar {
public:
	ScrollBar(Vector2 position);
	~ScrollBar();

	bool initialize(ID3D11Device* device,
		ID3D11ShaderResourceView* pixelTexture, size_t maxHeight);

	void update(double deltaTime, MouseController* mouse);
	void draw(SpriteBatch* batch);

	void setScrollBar(int totalListHeight);

	int getWidth();

	float percentScroll = 0;
private:

	/* Position of entire scrollbar area. */
	Vector2 position;

	RECT scrollBarRect;
	/* Position of bar part of scroll bar (minus buttons) */
	Vector2 scrollBarPosition;
	/*RECT scrubberRect;
	Vector2 scrubberPosition;
	unique_ptr<HitArea> scrubberHitArea;*/
	unique_ptr<Scrubber> scrubber;

	int maxHeight;

	unique_ptr<ImageButton> scrollBarUpButton;
	unique_ptr<ImageButton> scrollBarDownButton;

	ID3D11ShaderResourceView* pixel;

};

/** A simple control to display various (text) items. */
class ListBox {
public:
	ListBox(const Vector2& position, const int width);
	~ListBox();

	bool initialize(ID3D11Device* device, const wchar_t* fontFile);

	void addItems(vector<wstring> items);

	void update(double deltaTime, MouseController* mouse);
	void draw(SpriteBatch* batch);
	void drawFrame(SpriteBatch* batch);

	void drawSelected(SpriteBatch* batch, const Vector2& selectedPosition);



	bool multiSelect = false;
	/* Max items to display before showing scroll bar. */
	size_t maxDisplayItems = 7;

private:



	/* width of listbox */
	int width;
	Vector2 position;
	/* Always smaller or equal to maxDisplayItems. */
	size_t itemsToDisplay;

	unique_ptr<FontSet> font;
	vector<ListItem*> listItems;
	size_t itemSelected;

	size_t itemHeight = 32;
	Vector2 firstItemPos;

	unique_ptr<ScrollBar> scrollBar;

	int firstItemToDisplay = 0;

	ComPtr<ID3D11ShaderResourceView> whiteBG;

	int frameThickness = 2;


};


