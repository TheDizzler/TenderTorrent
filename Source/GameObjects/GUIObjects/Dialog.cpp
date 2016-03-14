#include "Dialog.h"

Dialog::Dialog(const Vector2& position) : Sprite(position) {


}

Dialog::~Dialog() {
	for (Button* button : buttons)
		delete button;

	for (TextLabel* label : labels)
		delete label;
}


bool Dialog::initialize(ID3D11Device * device, const wchar_t* fontFile) {

	if (!Sprite::load(device, L"assets/dialog pink (200x200).dds"))
		return false;

	setScale(Vector2(3, 1.5));

	font.reset(new FontSet());
	if (!font->load(device, fontFile))
		return false;
	font->setTint(DirectX::Colors::White.v);

	Vector2 textLoc;
	Vector2 okBtn;
	Vector2 cancelBtn;


	TextLabel* label = new TextLabel(textLoc, font.get());
	label->setText("Really Quit Tender Torrent?");
	Vector2 size = font->measureString(label->getText());
	textLoc = Vector2(position.x - size.x / 2, position.y - height / 4);
	label->position = textLoc;
	labels.push_back(label);


	Vector2 scaleFactor = Vector2(.75, 1);
	Button* button = new Button();
	if (!button->load(device, fontFile,
		Assets::buttonUpFile, Assets::buttonDownFile))
		return false;
	button->action = ButtonAction::OK;
	button->setScale(scaleFactor);
	button->setText("Quit");
	okBtn = Vector2(position.x - width + button->getWidth() / 3,
		position.y + height / 4);
	button->setPosition(okBtn);
	buttons.push_back(button);


	button = new Button();
	if (!button->load(device, fontFile,
		Assets::buttonUpFile, Assets::buttonDownFile))
		return false;
	button->action = ButtonAction::CANCEL_BUTTON;
	button->setScale(scaleFactor);
	button->setText("Keep Playing!");
	cancelBtn = Vector2(position.x + width - button->getWidth() / 3,
		position.y + height / 4);
	button->setPosition(cancelBtn);
	buttons.push_back(button);

	return true;
}

void Dialog::update(double deltaTime, MouseController* mouse) {

	result = NONE;

	for (Button* button : buttons) {
		button->update(deltaTime, mouse);
		if (button->clicked()) {
			switch (button->action) {
				case OK:
					result = CONFIRM;
					break;
				case ButtonAction::CANCEL_BUTTON:
					result = DialogResult::CANCEL_DIALOG;
					break;
			}
		}
	}
}

void Dialog::open() {
	isOpen = true;
}

void Dialog::close() {
	isOpen = false;
}

DialogResult Dialog::getResult() {
	return result;
}

void Dialog::draw(SpriteBatch* batch) {

	Sprite::draw(batch);

	for (Button* button : buttons)
		button->draw(batch);

	for (TextLabel* label : labels)
		label->draw(batch);
}


