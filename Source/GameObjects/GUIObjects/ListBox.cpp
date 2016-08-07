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

	/*wostringstream ws;
	ws << "\n" << "x: " << position.x;
	ws << " y: " << position.y << "\n";

	OutputDebugString(ws.str().c_str());*/

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

	//Vector2 pos = firstItemPos;

	for (wstring item : items) {

		ListItem* listItem = new ListItem(
			width - scrollBar->getWidth(), itemHeight,
			font.get(), whiteBG.Get());
		listItem->setText(item);
		listItems.push_back(listItem);
		//pos.y += itemHeight;
	}

	itemsToDisplay = maxDisplayItems;
	if (listItems.size() < itemsToDisplay)
		itemsToDisplay = listItems.size();

	scrollBar->setScrollBar(listItems.size() * itemHeight);
}


void ListBox::update(double deltaTime, MouseController* mouse) {


	for (ListItem* item : listItems) {
		if (item->update(deltaTime, mouse) && !multiSelect) {
			for (ListItem* unselect : listItems) {
				if (unselect == item)
					continue;
				unselect->isSelected = false;
			}
		}
	}

	if (itemsToDisplay == maxDisplayItems) {
		scrollBar->update(deltaTime, mouse);
		firstItemToDisplay = (scrollBar->percentScroll)
			* listItems.size();

		/*wostringstream ws;
		ws << "\n" << "%: " << scrollBar->percentScroll;
		OutputDebugString(ws.str().c_str());*/

		if (firstItemToDisplay > listItems.size() - maxDisplayItems)
			firstItemToDisplay = listItems.size() - maxDisplayItems;
	}

	Vector2 pos = firstItemPos;

	for (int i = firstItemToDisplay;
		i < firstItemToDisplay + itemsToDisplay; ++i) {

		listItems[i]->updatePosition(pos);
		pos.y += itemHeight;
	}
}


void ListBox::draw(SpriteBatch* batch) {

	size_t shown = 0;
	//for (ListItem* listItem : listItems) {
	for (int i = firstItemToDisplay;
		i < firstItemToDisplay + itemsToDisplay; ++i) {

		listItems[i]->draw(batch);
		//if (++shown >= maxDisplayItems) {
		//	// display scrollbar
		//	scrollBar->draw(batch);
		//	break;
		//}
	}

	if (itemsToDisplay == maxDisplayItems)
		scrollBar->draw(batch);

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
/** **** ListBox END **** **/


/** **** ListItem **** **/
ListItem::ListItem(/*Vector2 pos, */const int width, const int height,
	FontSet* font, ID3D11ShaderResourceView* pixelTexture) : TextLabel(font) {

	itemRect.left = 0;
	itemRect.top = 0;
	itemRect.bottom = height;
	itemRect.right = width;

	hitArea.reset(new HitArea(
		Vector2::Zero, Vector2(width, height)));

	pixel = pixelTexture;
	itemPosition = Vector2::Zero;

	/*position.x += textMarginX;
	position.y += textMarginY;*/
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

void ListItem::updatePosition(const Vector2 & pos) {

	itemPosition = pos;
	hitArea->position = itemPosition;

	position = pos;
	position.x += textMarginX;
	position.y += textMarginY;
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
/** **** ListItem END **** **/



/** **** SCROLLBAR **** **/
ScrollBar::ScrollBar(Vector2 pos) {

	position = pos;
}

ScrollBar::~ScrollBar() {
}

bool ScrollBar::initialize(ID3D11Device* device,
	ID3D11ShaderResourceView* pixelTexture, size_t maxHght) {

	maxHeight = maxHght;

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
	scrollBarRect.left = 0;
	scrollBarRect.top = 0;
	scrollBarRect.bottom = maxHeight
		- scrollBarUpButton->getHeight() * 2;
	scrollBarRect.right = scrollBarUpButton->getWidth();


	/*scrubberPosition = scrollBarPosition;
	scrubberRect = scrollbarRect;*/

	pixel = pixelTexture;

	Vector2 scrubberStartPos(
		scrollBarPosition.x,
		scrollBarPosition.y);

	scrubber.reset(new Scrubber(pixel));
	scrubber->setDimensions(scrubberStartPos,
		Vector2(scrollBarRect.right, scrollBarRect.bottom),
		scrollBarRect.bottom);

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
	scrubber->update(deltaTime, mouse);
	percentScroll = scrubber->percentAt;
}

void ScrollBar::draw(SpriteBatch * batch) {

	scrollBarDownButton->draw(batch);
	scrollBarUpButton->draw(batch);

	// draw bar
	batch->Draw(pixel, scrollBarPosition, &scrollBarRect,
		DirectX::Colors::Gray.v, 0.0f, Vector2(0, 0), Vector2(1, 1),
		SpriteEffects_None, 0.0f);

	// draw scrubber
	scrubber->draw(batch);

}

void ScrollBar::setScrollBar(int totalListHeight) {

	float percentToShow = maxHeight / (float) totalListHeight;

	//scrubberRect.bottom *= percentToShow;
	Vector2 newSize(scrubber->getWidth(), scrubber->getHeight() * percentToShow);
	scrubber->setSize(newSize);
	/*wostringstream ws;
	ws << "%: " << percentToShow;
	ws << "    bottom: " << scrubberRect.bottom;
	MessageBox(0, ws.str().c_str(), L"Test", MB_OK);*/
}


int ScrollBar::getWidth() {
	return scrollBarDownButton->getWidth();
}
/** **** ScrollBar END **** **/


/** **** Scrubber **** **/
Scrubber::Scrubber(ID3D11ShaderResourceView* pixel)
	: RectangleSprite(pixel) {
}

Scrubber::~Scrubber() {
}

void Scrubber::setDimensions(const Vector2& startPos, const Vector2& size,
	const int scrllBrHght) {

	width = size.x;
	height = size.y;

	minPosition = startPos;

	scrollBarHeight = scrllBrHght;

	maxPosition = startPos;
	maxPosition.y += scrollBarHeight - height / 2;

	position = minPosition;


	sourceRect.left = 0;
	sourceRect.top = 0;
	sourceRect.bottom = height;
	sourceRect.right = width;

	hitArea.reset(new HitArea(minPosition,
		Vector2(width*scale.x, height*scale.y)));

}


void Scrubber::setSize(const Vector2& size) {

	width = size.x;
	height = size.y;

	sourceRect.left = 0;
	sourceRect.top = 0;
	sourceRect.bottom = height;
	sourceRect.right = width;

	maxPosition = minPosition;
	maxPosition.y += scrollBarHeight - height;

	hitArea.reset(new HitArea(minPosition,
		Vector2(width*scale.x, height*scale.y)));

	minMaxDifference = maxPosition.y - minPosition.y;
}


void Scrubber::update(double deltaTime, MouseController* mouse) {

	isHover = hitArea->contains(mouse->getPosition());

	if (isHover && mouse->leftButtonDown() && !mouse->leftButtonLastDown()) {
		isPressed = true;
		pressedPosition = mouse->getPosition().y - position.y;

	} else if (!mouse->leftButtonDown())
		isPressed = false;

	if (isPressed) {
		tint = selectedColor;
		position.y = mouse->getPosition().y - pressedPosition;

		if (position.y < minPosition.y)
			position.y = minPosition.y;
		else if (position.y > maxPosition.y)
			position.y = maxPosition.y;

		hitArea->position = Vector2(position.x, position.y);

		float distanceBetweenPosAndMax = maxPosition.y - position.y;
		percentAt = (minMaxDifference - distanceBetweenPosAndMax)
			/ (minMaxDifference * 2);

	} else if (isHover)
		tint = hoverColor;
	else
		tint = normalColor;
}

//void Scrubber::draw(SpriteBatch* batch) {
//
//
//}



bool Scrubber::pressed() {
	/*if (isPressed) {
		isPressed = false;
		return true;
	}*/
	return isPressed;
}
/** **** Scrubber END **** **/