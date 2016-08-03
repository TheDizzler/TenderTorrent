#pragma once

#include "ListBox.h"


class ComboBox {
public:
	ComboBox(const Vector2& position, const int length);
	~ComboBox();

	virtual bool initialize(ID3D11Device* device, const wchar_t* fontFile);
	virtual void update(double deltaTime, MouseController* mouse);
	virtual void draw(SpriteBatch* batch);


	void addItems(vector<wstring> items);

	void open();
	void close();
	bool isOpen = false;

private:
	unique_ptr<ListBox> listBox;

	// unique_ptr<Sprite> buttonSprite;
	unique_ptr<Button> buttonClosed;
	unique_ptr<Button> buttonOpen;
	Button* button;

	Vector2 position;

	Vector2 selectedPosition;

	// length of combobox
	int length;

	size_t textMarginX = 10;
	size_t textMarginY = 10;

};