#include "ListBox.h"


ListBox::ListBox(const Vector2& pos, const int len) {

	position = pos;
	width = len;

}

ListBox::~ListBox() {

	for (ListItem* listItem : listItems)
		delete listItem;

}


#include "DDSTextureLoader.h"
#include "../../globals.h"
bool ListBox::initialize(ID3D11Device* device, const wchar_t* fontFile) {


	font.reset(new FontSet());
	if (!font->load(device, fontFile))
		return false;
	font->setTint(DirectX::Colors::White.v);


	if (Globals::reportError(DirectX::CreateDDSTextureFromFile(
		device, Assets::whitePixelFile, NULL, whiteBG.GetAddressOf()))) {

		MessageBox(NULL, L"Failed to create texture from WhitePixel.dds",
			L"ERROR", MB_OK);
		return false;
	}

	//spaceBetweenItems = 32;
	firstItemPos = Vector2(position.x, position.y);

	scrollBar.reset(new ScrollBar(Vector2(position.x + width, position.y)));
	if (!scrollBar->initialize(device, whiteBG.Get(),
		itemHeight * maxDisplayItems)) {

		MessageBox(NULL, L"Failed to create ScrollBar",
			L"GUI initialization ERROR", MB_OK);
		return false;
	}



	return true;
}



void ListBox::addItems(vector<wstring> items) {

	Vector2 pos = firstItemPos;

	for (wstring item : items) {

		ListItem* listItem = new ListItem(pos,
			width - scrollBar->getWidth(), itemHeight,
			font.get(), whiteBG.Get());
		listItem->setText(item);
		listItems.push_back(listItem);
		pos.y += itemHeight;
	}

	itemsToDisplay = maxDisplayItems;
	if (listItems.size() < itemsToDisplay)
		itemsToDisplay = listItems.size();
}


void ListBox::update(double deltaTime, MouseController* mouse) {

	for (ListItem* item : listItems) {
		if (item->update(deltaTime, mouse)) {
			for (ListItem* unselect : listItems) {
				if (unselect == item)
					continue;
				unselect->isSelected = false;
			}
		}
	}

	scrollBar->update(deltaTime, mouse);
}


void ListBox::draw(SpriteBatch* batch) {

	size_t shown = 0;
	for (ListItem* listItem : listItems) {

		listItem->draw(batch);
		if (++shown >= maxDisplayItems) {
			// display scrollbar
			scrollBar->draw(batch);
			break;
		}
	}

	if (listItems.size() > 0) { // draw frame
		drawFrame(batch);
	}

}


void ListBox::drawFrame(SpriteBatch* batch) {

	// upper horizontal frame
	RECT frame;
	frame.left = 0;
	frame.top = 0;
	frame.right = width;
	frame.bottom = frameThickness; // thickness of frame
	Vector2 framePos(position.x, position.y);

	batch->Draw(whiteBG.Get(), framePos, &frame,
		DirectX::Colors::Black.v, 0.0f, Vector2(0, 0), Vector2(1, 1),
		SpriteEffects_None, 0.0f);

	// lower horizontal frame
	int height = itemHeight * itemsToDisplay;
	framePos.y += height;

	batch->Draw(whiteBG.Get(), framePos, &frame,
		DirectX::Colors::Black.v, 0.0f, Vector2(0, 0), Vector2(1, 1),
		SpriteEffects_None, 0.0f);

	// left vertical frame
	framePos.y = position.y;
	frame.right = frameThickness;
	frame.bottom = height;

	batch->Draw(whiteBG.Get(), framePos, &frame,
		DirectX::Colors::Black.v, 0.0f, Vector2(0, 0), Vector2(1, 1),
		SpriteEffects_None, 0.0f);

	// right vertical frame
	framePos.x += width;

	batch->Draw(whiteBG.Get(), framePos, &frame,
		DirectX::Colors::Black.v, 0.0f, Vector2(0, 0), Vector2(1, 1),
		SpriteEffects_None, 0.0f);

}

void ListBox::drawSelected(SpriteBatch* batch, const Vector2& selectedPosition) {

	font->draw(batch, listItems[itemSelected]->getText(), selectedPosition);
}







ListItem::ListItem(Vector2 pos, const int width, const int height,
	FontSet* font, ID3D11ShaderResourceView* pixelTexture) : TextLabel(pos, font) {

	itemRect.left = 0;
	itemRect.top = 0;
	itemRect.bottom = height;
	itemRect.right = width;

	hitArea.reset(new HitArea(
		Vector2(pos.x, pos.y),
		Vector2(width, height)));

	pixel = pixelTexture;
	itemPosition = pos;

	position.x += textMarginX;
	position.y += textMarginY;
}

ListItem::~ListItem() {
}

bool ListItem::update(double deltaTime, MouseController* mouse) {

	if ((isHover = hitArea->contains(mouse->getPosition())) == true) {

		if (mouse->leftButtonDown() && !buttonDownLast)
			buttonDownLast = true;

		else if (!mouse->leftButtonDown() && buttonDownLast) {
			isSelected = true;
			buttonDownLast = false;
		}

	} else
		buttonDownLast = false;

	return isSelected;
}


void ListItem::draw(SpriteBatch* batch) {

	if (isSelected) {// draw selected color bg

		batch->Draw(pixel, itemPosition, &itemRect,
			DirectX::Colors::White.v, 0.0f, Vector2(0, 0), Vector2(1, 1),
			SpriteEffects_None, 0.0f);
		TextLabel::draw(batch, DirectX::Colors::Black.v);

	} else if (isHover) { // draw hover color bg

		batch->Draw(pixel, itemPosition, &itemRect,
			DirectX::Colors::Aqua.v, 0.0f, Vector2(0, 0), Vector2(1, 1),
			SpriteEffects_None, 0.0f);
		TextLabel::draw(batch);

	} else { // draw basic bg

		batch->Draw(pixel, itemPosition, &itemRect,
			DirectX::Colors::BurlyWood.v, 0.0f, Vector2(0, 0), Vector2(1, 1),
			SpriteEffects_None, 0.0f);
		TextLabel::draw(batch);
	}
}


/** **** SCROLLBAR **** **/
ScrollBar::ScrollBar(Vector2 pos) {

	position = pos;
}

ScrollBar::~ScrollBar() {
}

bool ScrollBar::initialize(ID3D11Device* device,
	ID3D11ShaderResourceView* pixelTexture, size_t maxHeight) {

	scrollBarDownButton.reset(new ImageButton());
	if (!scrollBarDownButton->load(device, Assets::scrollBarDownFile,
		Assets::scrollBarDownPressedFile)) {
		MessageBox(NULL, L"Unable to load ScrollBarDown",
			L"SPRITE LOAD ERROR", MB_OK);
		return false;
	}
	scrollBarDownButton->setPosition(
		Vector2(position.x - scrollBarDownButton->getWidth() / 2,
			position.y + maxHeight
			- scrollBarDownButton->getHeight() / 2));

	scrollBarDownButton->action = DOWN;


	scrollBarUpButton.reset(new ImageButton());
	if (!scrollBarUpButton->load(device, Assets::scrollBarUpFile,
		Assets::scrollBarUpPressedFile)) {
		MessageBox(NULL, L"Unable to load ScrollBarUp",
			L"SPRITE LOAD ERROR", MB_OK);
		return false;
	}
	scrollBarUpButton->setPosition(
		Vector2(position.x - scrollBarUpButton->getWidth() / 2,
			position.y + scrollBarUpButton->getHeight() / 2));


	scrollBarUpButton->action = UP;

	scrollBarPosition =
		Vector2(position.x - scrollBarUpButton->getWidth(),
			position.y + scrollBarUpButton->getHeight());
	scrollbarRect.left = 0;
	scrollbarRect.top = 0;
	scrollbarRect.bottom = maxHeight
		- scrollBarUpButton->getHeight() * 2;
	scrollbarRect.right = scrollBarUpButton->getWidth();

	pixel = pixelTexture;

	return true;
}

void ScrollBar::update(double deltaTime, MouseController* mouse) {

	scrollBarDownButton->update(deltaTime, mouse);
	if (scrollBarDownButton->clicked()) {
		// scroll down
	}

	scrollBarUpButton->update(deltaTime, mouse);
	if (scrollBarUpButton->clicked()) {
		// scroll up
	}

	// update scrubber
}

void ScrollBar::draw(SpriteBatch * batch) {

	scrollBarDownButton->draw(batch);
	scrollBarUpButton->draw(batch);
	batch->Draw(pixel, scrollBarPosition, &scrollbarRect,
		DirectX::Colors::Gray.v, 0.0f, Vector2(0, 0), Vector2(1, 1),
		SpriteEffects_None, 0.0f);

	// draw scrubber
}

int ScrollBar::getWidth() {
	return scrollBarDownButton->getWidth();
}
