#include "ListBox.h"


ListBox::ListBox(const Vector2& pos, const int len) {

	position = pos;
	length = len;



}

ListBox::~ListBox() {
}


#include "DDSTextureLoader.h"
bool ListBox::initialize(ID3D11Device* device, const wchar_t* fontFile) {

	font.reset(new FontSet());
	if (!font->load(device, fontFile))
		return false;
	font->setTint(DirectX::Colors::White.v);

	//spaceBetweenItems = 32;
	firstItemPos = Vector2(position.x + textMarginX, position.y + textMarginY);

	if (Globals::reportError(DirectX::CreateDDSTextureFromFile(
		device, Assets::whitePixelFile, NULL, whiteBG.GetAddressOf()))) {
		MessageBox(NULL, L"Failed to create texture from WhitePixel.dds ", L"ERROR", MB_OK);
		return false;
	}

	return true;
}


void ListBox::update(double deltaTime, MouseController* mouse) {

	for each (ListItem* item in listItems)
		item->update(deltaTime, mouse);

}


void ListBox::draw(SpriteBatch* batch) {

	for each (ListItem* listItem in listItems) {

		//batch->Draw(Assets::whiteBG, label->draw(batch);
		listItem->draw(batch);

	}
}

void ListBox::drawSelected(SpriteBatch* batch, const Vector2& selectedPosition) {

	font->draw(batch, listItems[itemSelected]->getText(), selectedPosition);
}



void ListBox::addItems(vector<wstring> items) {

	Vector2 pos = firstItemPos;

	for each (wstring item in items) {

		ListItem* listItem = new ListItem(pos, length, spaceBetweenItems,
			font.get(), whiteBG.Get());
		listItem->setText(item);
		listItems.push_back(listItem);
		pos.y += spaceBetweenItems;
	}

}




ListItem::ListItem(Vector2 pos, const int length, const int height,
	FontSet* font, ID3D11ShaderResourceView* pixelTexture) : TextLabel(pos, font) {

	itemRect.left = 0;
	itemRect.top = 0;
	itemRect.bottom = height;
	itemRect.right = length;

	hitArea.reset(new HitArea(
		Vector2(pos.x - length / 2, pos.y - height / 2),
		Vector2(length, height)));

	pixel = pixelTexture;
}

ListItem::~ListItem() {
}

void ListItem::update(double deltaTime, MouseController* mouse) {

	isHover = hitArea->contains(mouse->getPosition());
}

void ListItem::draw(SpriteBatch * batch) {

	if (isSelected) // draw selected color bg
		batch->Draw(pixel, position, &itemRect,
			DirectX::Colors::White.v, 0.0f, Vector2(0, 0), Vector2(1, 1),
			SpriteEffects_None, 0.0f);
	else if (isHover) // draw hover color bg
		batch->Draw(pixel, position, &itemRect,
			DirectX::Colors::Aqua.v, 0.0f, Vector2(0, 0), Vector2(1, 1),
			SpriteEffects_None, 0.0f);
	else // draw basic bg
		batch->Draw(pixel, position, &itemRect,
			DirectX::Colors::BurlyWood.v, 0.0f, Vector2(0, 0), Vector2(1, 1),
			SpriteEffects_None, 0.0f);

	TextLabel::draw(batch); // draw actual text

}
