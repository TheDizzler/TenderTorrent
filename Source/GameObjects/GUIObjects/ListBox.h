#pragma once

#include <vector>

#include "TextLabel.h"
#include "ImageButton.h"

using namespace std;

class ListItem : public TextLabel {
public:

	ListItem(Vector2 pos, const int width, const int height,
		FontSet* fnt, ID3D11ShaderResourceView* pixelTexture);
	~ListItem();


	bool update(double deltaTime, MouseController* mouse);
	virtual void draw(SpriteBatch* batch);

	bool isSelected = false;

private:

	RECT itemRect;
	unique_ptr<HitArea> hitArea;

	// position is for actual text only
	Vector2 itemPosition;

	size_t textMarginX = 10;
	size_t textMarginY = 5;

	bool isHover = false;
	bool buttonDownLast = false;

	ID3D11ShaderResourceView* pixel;

};

class ScrollBar {
public:
	ScrollBar(Vector2 position);
	~ScrollBar();

	bool initialize(ID3D11Device* device,
		ID3D11ShaderResourceView* pixelTexture, size_t maxHeight);

	void update(double deltaTime, MouseController* mouse);
	void draw(SpriteBatch* batch);

	int getWidth();

private:

	Vector2 position;

	RECT scrollbarRect;
	Vector2 scrollBarPosition;
	RECT scrubberRect;
	Vector2 scrubberPositon;

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



	ComPtr<ID3D11ShaderResourceView> whiteBG;

	int frameThickness = 2;


};


