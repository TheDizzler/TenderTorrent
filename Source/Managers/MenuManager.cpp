#include "MenuManager.h"
#include "GameManager.h"


MenuManager::MenuManager() {
}

MenuManager::~MenuManager() {
}

void MenuManager::setGameManager(GameManager * gm) {

	game = gm;


}

bool MenuManager::initialize(ID3D11Device* device, MouseController* mouse) {

	menuFont.reset(new FontSet());
	if (!menuFont->load(device, Assets::arialFontFile))
		return false;
	menuFont->setTint(DirectX::Colors::Black.v);

	if (!mouse->load(device, L"assets/mouse icon.dds"))
		return false;

	mainScreen.reset(new MainScreen(this, menuFont.get()));
	mainScreen->setGameManager(game);
	if (!mainScreen->initialize(device, mouse))
		return false;

	configScreen.reset(new ConfigScreen(this, menuFont.get()));
	configScreen->setGameManager(game);
	if (!configScreen->initialize(device, mouse))
		return false;



	currentScreen = mainScreen.get();
	return true;
}

bool lastStateDown;

void MenuManager::update(double deltaTime, BYTE keyboardState[256], MouseController* mouse) {

	Vector2 mousePos = mouse->getPosition();
	if (mousePos.x > Globals::WINDOW_WIDTH) {
		mousePos.x = Globals::WINDOW_WIDTH;
		SetCursorPos(mousePos.x, mousePos.y);
	}
	if (mousePos.y > Globals::WINDOW_HEIGHT) {
		mousePos.y = Globals::WINDOW_HEIGHT;
		SetCursorPos(mousePos.x, mousePos.y);
	}
	if (mousePos.x < 0) {
		mousePos.x = 0;
		SetCursorPos(mousePos.x, mousePos.y);
	}
	if (mousePos.y < 0) {
		mousePos.y = 0;
		SetCursorPos(mousePos.x, mousePos.y);
	}

	currentScreen->update(deltaTime, keyboardState, mouse);

}


void MenuManager::draw(SpriteBatch* batch) {

	currentScreen->draw(batch);

}

void MenuManager::openMainMenu() {

	currentScreen = mainScreen.get();
}

void MenuManager::openConfigMenu() {

	currentScreen = configScreen.get();
}


/** **** MenuScreen abstract class **** */
MenuScreen::MenuScreen(MenuManager * mngr, FontSet * fntst) {

	menuManager = mngr;
	menuFont = fntst;
}

MenuScreen::~MenuScreen() {

// textlabels are unique_ptrs
	/*for each (TextLabel* label in textLabels)
		if (label)
			delete label;*/

	for (TextButton* button : buttons)
		delete button;

	for (ListBox* list : listBoxes)
		delete list;
}

void MenuScreen::setGameManager(GameManager* gmMng) {

	game = gmMng;
}


/** **** MainMenuScreen **** **/
MainScreen::MainScreen(MenuManager* mngr, FontSet* fntst) : MenuScreen(mngr, fntst) {

}

MainScreen::~MainScreen() {
}


bool MainScreen::initialize(ID3D11Device* device, MouseController* mouse) {

	TextButton* button = new TextButton();
	if (!button->load(device, Assets::arialFontFile,
		Assets::buttonUpFile, Assets::buttonDownFile))
		return false;
	button->action = PLAY;
	button->setText("Play");
	button->setPosition(Vector2(Globals::WINDOW_WIDTH / 2, 200));
		//Globals::WINDOW_HEIGHT - button->getHeight())
	buttons.push_back(button);


	button = new TextButton();
	if (!button->load(device, Assets::arialFontFile,
		Assets::buttonUpFile, Assets::buttonDownFile))
		return false;
	button->action = SETTINGS;
	button->setText("Settings");
	button->setPosition(Vector2(Globals::WINDOW_WIDTH / 2, 350));
	buttons.push_back(button);


	button = new TextButton();
	if (!button->load(device, Assets::arialFontFile,
		Assets::buttonUpFile, Assets::buttonDownFile))
		return false;
	button->action = EXIT;
	button->setText("Exit");
	button->setPosition(Vector2(Globals::WINDOW_WIDTH / 2, 500));
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
		for (TextButton* button : buttons) {
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
						menuManager->openConfigMenu();
						//test->setText("Settings!!");
						break;
				}
			}
		}
	}
}


void MainScreen::draw(SpriteBatch* batch) {

	for (TextButton* button : buttons)
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
ConfigScreen::ConfigScreen(MenuManager* mngr, FontSet* fntst) : MenuScreen(mngr, fntst) {

}

ConfigScreen::~ConfigScreen() {
}


bool ConfigScreen::initialize(ID3D11Device* device, MouseController* mouse) {

	ListBox* listbox = new ListBox(Vector2(50, 100), 400);
	listbox->initialize(device, Assets::arialFontFile);
	listbox->addItems(game->getAdapters());
	listBoxes.push_back(listbox);

	// Selected adapter display mode list
	listbox = new ListBox(Vector2(475, 100), 150);
	listbox->initialize(device, Assets::arialFontFile);
	listbox->addItems(game->getDisplayModeList(game->getSelectedAdapterIndex()));
	listBoxes.push_back(listbox);


	TextButton* button = new TextButton();
	if (!button->load(device, Assets::arialFontFile,
		Assets::buttonUpFile, Assets::buttonDownFile))
		return false;
	button->action = ButtonAction::CANCEL_BUTTON;
	button->setText("Back");
	button->setPosition(
		Vector2(Globals::WINDOW_WIDTH / 2 - button->getWidth(),
			Globals::WINDOW_HEIGHT - button->getHeight()));
	buttons.push_back(button);

	button = new TextButton();
	if (!button->load(device, Assets::arialFontFile,
		Assets::buttonUpFile, Assets::buttonDownFile))
		return false;
	button->action = ButtonAction::OK;
	button->setText("Apply");
	button->setPosition(
		Vector2(Globals::WINDOW_WIDTH / 2 + button->getWidth(),
			Globals::WINDOW_HEIGHT - button->getHeight()));
	buttons.push_back(button);


	return true;
}


void ConfigScreen::update(double deltaTime, BYTE keyboardState[256],
	MouseController* mouse) {

	for (TextButton* button : buttons) {
		button->update(deltaTime, mouse);
		if (button->clicked()) {
			//test->setText("Clicked!");
			switch (button->action) {
				case CANCEL_BUTTON:
					menuManager->openMainMenu();
					break;
			}
		}
	}

	for (ListBox* listbox : listBoxes) {

		listbox->update(deltaTime, mouse);
	}
}

void ConfigScreen::draw(SpriteBatch* batch) {

	for (TextButton* button : buttons)
		button->draw(batch);

	for (ListBox* listbox : listBoxes)
		listbox->draw(batch);
}

