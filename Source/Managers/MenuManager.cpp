#include "MenuManager.h"

MenuManager::MenuManager() {
}

MenuManager::~MenuManager() {
}

bool MenuManager::initialize(ID3D11Device * device, MouseController* mouse) {

	menuFont.reset(new FontSet());
	if (!menuFont->load(device, L"assets/Arial.spritefont"))
		return false;
	menuFont->setTint(DirectX::Colors::Black.v);

	if (!mouse->load(device, L"assets/mouse icon.dds"))
		return false;

	Button* button = new Button(device, L"assets/Arial.spritefont");
	if (!button->load(device, L"assets/big button.dds"))
		return false;

	button->setText("This is a Button");
	button->setPosition(Vector2(200, 200));
	buttons.push_back(button);


	test.reset(new TextLabel(Vector2(10, 10), menuFont.get()));

	textLabels.push_back(test.get());

	mouseLabel.reset(new TextLabel(Vector2(10, 100), menuFont.get()));
	textLabels.push_back(mouseLabel.get());

	return true;
}

void MenuManager::update(double deltaTime, BYTE keyboardState[256], MouseController* mouse) {

	// GUI updates

	wostringstream ws;
	ws << "Mouse: " << mouse->position.x << ", " << mouse->position.y;
	mouseLabel->label = ws.str();
	

	for (Button* button : buttons) {
		button->update(deltaTime, mouse);
		if (button->clicked()) {
			test->setText("Clicked!");
		}
	}

}

void MenuManager::draw(SpriteBatch * batch) {

	for (Button* button : buttons)
		button->draw(batch);

	for (auto const& label : textLabels)
		label->draw(batch);
}
