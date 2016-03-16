#include "MenuManager.h"
#include "GameManager.h"


MenuManager::MenuManager() {
}

MenuManager::~MenuManager() {
}

void MenuManager::setGameManager(GameManager * gm) {

	game = gm;
}

bool MenuManager::initialize(ID3D11Device * device, MouseController* mouse) {

	menuFont.reset(new FontSet());
	if (!menuFont->load(device, Assets::arialFontFile))
		return false;
	menuFont->setTint(DirectX::Colors::Black.v);

	if (!mouse->load(device, L"assets/mouse icon.dds"))
		return false;

	Button* button = new Button();
	if (!button->load(device, Assets::arialFontFile,
		Assets::buttonUpFile, Assets::buttonDownFile))
		return false;
	button->action = PLAY;
	button->setText("Play");
	button->setPosition(Vector2(200, 200));
	buttons.push_back(button);

	button = new Button();
	if (!button->load(device, Assets::arialFontFile,
		Assets::buttonUpFile, Assets::buttonDownFile))
		return false;
	button->action = EXIT;
	button->setText("Exit");
	button->setPosition(Vector2(600, 200));
	buttons.push_back(button);


	test.reset(new TextLabel(Vector2(10, 10), menuFont.get()));

	textLabels.push_back(test.get());

	mouseLabel.reset(new TextLabel(Vector2(10, 100), menuFont.get()));
	textLabels.push_back(mouseLabel.get());


	exitDialog.reset(new Dialog(
		Vector2(Globals::WINDOW_WIDTH / 2, Globals::WINDOW_HEIGHT / 2)));
	if (!exitDialog->initialize(device, Assets::arialFontFile)) {
		MessageBox(0, L"Dialog init failed", L"Error", MB_OK);
		return false;
	}


	return true;
}

bool lastStateDown;

void MenuManager::update(double deltaTime, BYTE keyboardState[256], MouseController* mouse) {

	// GUI updates

	wostringstream ws;
	ws << "Mouse: " << mouse->position.x << ", " << mouse->position.y;
	mouseLabel->setText(ws);


	if (keyboardState[DIK_ESCAPE] && !lastStateDown) {
		if (exitDialog->isOpen)
			exitDialog->close();
			//game->exit();
		else
			confirmExit();
	}

	lastStateDown = keyboardState[DIK_ESCAPE];

	if (exitDialog->isOpen) {
		exitDialog->update(deltaTime, mouse);
		switch (exitDialog->getResult()) {
			case CONFIRM:
				game->exit();
				break;
			case DialogResult::CANCEL_DIALOG:
				exitDialog->close();
				break;
		}

	} else {
		for (Button* button : buttons) {
			button->update(deltaTime, mouse);
			if (button->clicked()) {
				//test->setText("Clicked!");
				switch (button->action) {
					case EXIT:
						//game->quit();
						confirmExit();
						//test->setText("Exit!");
						break;
					case PLAY:
						game->loadLevel();
						//test->setText("Play!");
						break;
				}
			}
		}
	}
}


void MenuManager::draw(SpriteBatch * batch) {

	for (Button* button : buttons)
		button->draw(batch);

	for (auto const& label : textLabels)
		label->draw(batch);

	if (exitDialog->isOpen)
		exitDialog->draw(batch);
}


void MenuManager::confirmExit() {

	exitDialog->open();

}
