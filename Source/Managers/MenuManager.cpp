#include "MenuManager.h"
#include "GameManager.h"


MenuManager::MenuManager() {
}

MenuManager::~MenuManager() {
}

void MenuManager::setGameManager(GameManager * gm) {

	game = gm;
	mainScreen->setGameManager(game);
	configScreen->setGameManager(game);
}

bool MenuManager::initialize(ID3D11Device* device, MouseController* mouse) {

	menuFont.reset(new FontSet());
	if (!menuFont->load(device, Assets::arialFontFile))
		return false;
	menuFont->setTint(DirectX::Colors::Black.v);

	if (!mouse->load(device, L"assets/mouse icon.dds"))
		return false;

	mainScreen.reset(new MainScreen(this, menuFont.get()));
	if (!mainScreen->initialize(device, mouse))
		return false;


	currentScreen = mainScreen.get();
	return true;
}

bool lastStateDown;

void MenuManager::update(double deltaTime, BYTE keyboardState[256], MouseController* mouse) {

	// GUI updates
	currentScreen->update(deltaTime, keyboardState, mouse);

}


void MenuManager::draw(SpriteBatch* batch) {

	currentScreen->draw(batch);
		/*for (Button* button : buttons)
			button->draw(batch);

		for (auto const& label : textLabels)
			label->draw(batch);

		if (exitDialog->isOpen)
			exitDialog->draw(batch);*/
}



/** **** MainScreen **** **/
MainScreen::MainScreen(MenuManager* mngr, FontSet* fntst) {

	manager = mngr;
	menuFont = fntst;
}


MainScreen::~MainScreen() {

	/*for each (TextLabel* label in textLabels)
		delete label;*/

	for each (Button* button in buttons)
		delete button;
}


bool MainScreen::initialize(ID3D11Device* device, MouseController* mouse) {

	Button* button = new Button();
	if (!button->load(device, Assets::arialFontFile,
		Assets::buttonUpFile, Assets::buttonDownFile))
		return false;
	button->action = PLAY;
	button->setText("Play");
	button->setPosition(Vector2(400, 200));
	buttons.push_back(button);


	button = new Button();
	if (!button->load(device, Assets::arialFontFile,
		Assets::buttonUpFile, Assets::buttonDownFile))
		return false;
	button->action = SETTINGS;
	button->setText("Settings");
	button->setPosition(Vector2(400, 350));
	buttons.push_back(button);


	button = new Button();
	if (!button->load(device, Assets::arialFontFile,
		Assets::buttonUpFile, Assets::buttonDownFile))
		return false;
	button->action = EXIT;
	button->setText("Exit");
	button->setPosition(Vector2(400, 500));
	buttons.push_back(button);


	test.reset(new TextLabel(Vector2(10, 10), menuFont));

	textLabels.push_back(test.get());

	mouseLabel.reset(new TextLabel(Vector2(10, 100), menuFont));
	textLabels.push_back(mouseLabel.get());


	exitDialog.reset(new Dialog(
		Vector2(Globals::WINDOW_WIDTH / 2, Globals::WINDOW_HEIGHT / 2)));
	if (!exitDialog->initialize(device, Assets::arialFontFile)) {
		MessageBox(0, L"Dialog init failed", L"Error", MB_OK);
		return false;
	}

	return true;
}


void MainScreen::setGameManager(GameManager* gmMng) {

	game = gmMng;
}


void MainScreen::update(double deltaTime, BYTE keyboardState[256], MouseController* mouse) {

	wostringstream ws;
	ws << "Mouse: " << mouse->getPosition().x << ", " << mouse->getPosition().y;
	mouseLabel->setText(ws);


	if (keyboardState[DIK_ESCAPE] && !lastStateDown) {
		if (exitDialog->isOpen)
			exitDialog->close();
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
						confirmExit();
						//test->setText("Exit!");
						break;
					case PLAY:
						game->loadLevel(Assets::levelMakoXML);
						//test->setText("Play!");
						break;
					case SETTINGS:


						break;
				}
			}
		}
	}
}


void MainScreen::draw(SpriteBatch* batch) {

	for (Button* button : buttons)
		button->draw(batch);

	for (auto const& label : textLabels)
		label->draw(batch);

	if (exitDialog->isOpen)
		exitDialog->draw(batch);
}


void MainScreen::confirmExit() {

	exitDialog->open();
}


/** **** ConfigScreen **** **/
ConfigScreen::ConfigScreen(MenuManager* mngr, FontSet* fntst) {

	manager = mngr;
	menuFont = fntst;
}

ConfigScreen::~ConfigScreen() {
}

bool ConfigScreen::initialize(ID3D11Device* device, MouseController* mouse) {
	return false;
}

void ConfigScreen::setGameManager(GameManager* gmMng) {

	game = gmMng;

}

void ConfigScreen::update(double deltaTime, BYTE keyboardState[256],
	MouseController* mouse) {

}

void ConfigScreen::draw(SpriteBatch * batch) {

}
