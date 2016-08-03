#pragma once

#include <vector>

#include "Button.h"

using namespace std;

class ListItem : public TextLabel {
public:

	ListItem(Vector2 pos, const int length, const int height,
		FontSet* fnt, ID3D11ShaderResourceView* pixelTexture);
	~ListItem();


	void update(double deltaTime, MouseController* mouse);
	virtual void draw(SpriteBatch* batch);

private:

	RECT itemRect;
	unique_ptr<HitArea> hitArea;

	bool isSelected = false;
	bool isHover = false;

	ID3D11ShaderResourceView* pixel;

};

/** A simple control to display various (text) items. */
class ListBox {
public:
	ListBox(const Vector2& position, const int length);
	~ListBox();

	bool initialize(ID3D11Device* device, const wchar_t* fontFile);

	void update(double deltaTime, MouseController* mouse);
	void draw(SpriteBatch* batch);
	void drawSelected(SpriteBatch* batch, const Vector2& selectedPosition);

	void addItems(vector<wstring> items);

private:


	// length of listbox
	int length;

	RECT itemRect;

	size_t textMarginX = 10;
	size_t textMarginY = 10;



	unique_ptr<FontSet> font;
	vector<ListItem*> listItems;
	size_t itemSelected;

	size_t spaceBetweenItems = 32;
	Vector2 firstItemPos;

	Vector2 position;

	//unique_ptr<Sprite> bgSprite;
	ComPtr<ID3D11ShaderResourceView> whiteBG;


};


