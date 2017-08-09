#include "../pch.h"
#include "MenuManager.h"
#include "GameManager.h"

MenuManager::MenuManager() {
}

MenuManager::~MenuManager() {
}

void MenuManager::setGameManager(GameManager* gm) {
	game = gm;
}

void MenuManager::reloadGraphicsAssets() {

	mainScreen->reloadGraphicsAssets();
	configScreen->reloadGraphicsAssets();
}


bool MenuManager::initialize(ComPtr<ID3D11Device> device, shared_ptr<MouseController> ms) {

	mouse = ms;

	if (!mouse->loadMouseIcon(guiFactory.get(), "Mouse Reticle"))
		return false;

	mainScreen.reset(new MainScreen(this));
	mainScreen->setGameManager(game);
	if (!mainScreen->initialize(device, mouse))
		return false;

	configScreen.reset(new ConfigScreen(this));
	configScreen->setGameManager(game);
	if (!configScreen->initialize(device, mouse))
		return false;
	configScreen->update(0);

	levelSelectScreen.reset(new LevelSelectScreen(this));
	if (!levelSelectScreen->initialize(device, mouse)) {
		return false;
	};

	currentScreen = mainScreen.get();

	//transitionManager.reset(
	//	new ScreenTransitions::ScreenTransitionManager(
	//		guiFactory.get(), "Default Transition BG"));
	//transitionManager->setTransition(
	//	//new ScreenTransitions::FlipScreenTransition(true));
	//	//new ScreenTransitions::SquareFlipScreenTransition());
	//	new ScreenTransitions::LineWipeScreenTransition());

	return true;
}


bool lastStateDown;
void MenuManager::update(double deltaTime) {


	//Vector2 mousePos = mouse->getPosition();
	//if (mousePos.x > Globals::WINDOW_WIDTH) {
	//	mousePos.x = Globals::WINDOW_WIDTH;
	//	//SetCursorPos(mousePos.x, mousePos.y);
	//	ShowCursor(true);
	//}
	//if (mousePos.y > Globals::WINDOW_HEIGHT) {
	//	mousePos.y = Globals::WINDOW_HEIGHT;
	//	//SetCursorPos(mousePos.x, mousePos.y);
	//	ShowCursor(true);
	//}
	//if (mousePos.x < 0) {
	//	mousePos.x = 0;
	//	//SetCursorPos(mousePos.x, mousePos.y);
	//	ShowCursor(true);
	//}
	//if (mousePos.y < 0) {
	//	mousePos.y = 0;
	//	//SetCursorPos(mousePos.x, mousePos.y);
	//	ShowCursor(true);
	//}

	/*if (switchTo != NULL) {
		if (transitionManager->runTransition(deltaTime)) {
			currentScreen = switchTo;
			switchTo = NULL;
		}
	} else*/
		currentScreen->update(deltaTime);

}


void MenuManager::draw(SpriteBatch* batch) {


	/*if (switchTo != NULL) {
		batch->Begin(SpriteSortMode_Deferred);
		{
			transitionManager->drawTransition(batch);
		}
		batch->End();
	} else*/
		currentScreen->draw(batch);


}

//void MenuManager::safedraw(SpriteBatch* batch) {
//	currentScreen->safedraw(batch);
//}

void MenuManager::pause() {
	// do nothing?
}

void MenuManager::openMainMenu() {

	switchTo = mainScreen.get();
	GameManager::transitionManager.transitionBetween(currentScreen, switchTo);
}

void MenuManager::openConfigMenu() {

	switchTo = configScreen.get();
	GameManager::transitionManager.transitionBetween(currentScreen, switchTo);
}

void MenuManager::openLevelSelectScreen() {

	switchTo = levelSelectScreen.get();
	GameManager::transitionManager.transitionBetween(currentScreen, switchTo);
}

void MenuManager::loadLevel(string levelXMLFile) {

	game->loadLevel(levelXMLFile);
}

void MenuManager::controllerRemoved(ControllerSocketNumber controllerSlot, PlayerSlotNumber slotNumber) {
}

void MenuManager::newController(shared_ptr<Joystick> newStick) {
}




/** **** MenuScreen abstract class **** */
MenuScreen::MenuScreen(MenuManager* mngr) {

	menuManager = mngr;
}

MenuScreen::~MenuScreen() {

	//for (GUIControl* control : guiControls)
		//delete control;

	guiControls.clear();
}

void MenuScreen::setGameManager(GameManager* gmMng) {

	game = gmMng;
}

void MenuScreen::pause() {
	// do nothing??
}

void MenuScreen::controllerRemoved(ControllerSocketNumber controllerSlot,
	PlayerSlotNumber slotNumber) {
}

void MenuScreen::newController(shared_ptr<Joystick> newStick) {
}


/** **** MainMenuScreen **** **/
MainScreen::MainScreen(MenuManager* mngr) : MenuScreen(mngr) {

}

MainScreen::~MainScreen() {
}


bool MainScreen::initialize(ComPtr<ID3D11Device> device, shared_ptr<MouseController> mouse) {

	Vector2 buttonpos = Vector2(Globals::WINDOW_WIDTH / 2, Globals::WINDOW_HEIGHT / 8);
	Vector2 buttonsize = Vector2(Globals::WINDOW_WIDTH / 4, 0);
	buttonpos.x -= buttonsize.x / 2;
	Button* button = guiFactory->createButton(
		buttonpos, buttonsize, L"Play");
	button->normalColor = Color(1, .558, 1, 1);
	button->setActionListener(new PlayButtonListener(this));
	guiControls.push_back(unique_ptr<GUIControl>(button));

	buttonpos.y = Globals::WINDOW_HEIGHT / 2 - button->getHeight() / 2;
	button = guiFactory->createButton(
		buttonpos, buttonsize, L"Settings");
	button->normalColor = Color(1, .558, 1, 1);
	button->setActionListener(new SettingsButtonListener(this));
	guiControls.push_back(unique_ptr<GUIControl>(button));

	buttonpos.y = Globals::WINDOW_HEIGHT - Globals::WINDOW_HEIGHT / 8 - button->getHeight();
	button = guiFactory->createButton(
		buttonpos, buttonsize, L"Exit");
	button->normalColor = Color(1, .558, 1, 1);
	button->setActionListener(new OnClickListenerExitButton(this));
	guiControls.push_back(unique_ptr<GUIControl>(button));



	{
		
		Vector2 dialogPos, dialogSize;
		dialogSize = Vector2(Globals::WINDOW_WIDTH / 2, Globals::WINDOW_HEIGHT / 2);
		dialogPos = dialogSize;
		dialogPos.x -= dialogSize.x / 2;
		dialogPos.y -= dialogSize.y / 2;
		exitDialog.reset(guiFactory->createDialog(dialogPos, dialogSize, true, true, 10));
		exitDialog->setTint(Color(0, .5, 1, 1));
		exitDialog->setTitle(L"Exit Test?", Vector2(1, 1), "BlackCloak");
		//exitDialog->setTitleAreaDimensions(Vector2(0, 150));
		exitDialog->setText(L"Really Quit The Test Project?");
		unique_ptr<Button> quitButton;
		quitButton.reset(guiFactory->createButton());
		quitButton->setActionListener(new OnClickListenerDialogQuitButton(this));
		quitButton->setText(L"Quit");
		exitDialog->setConfirmButton(move(quitButton));
		exitDialog->setCancelButton(L"Keep Testing!");
		exitDialog->setOpenTransition(
			new TransitionEffects::SpinGrowTransition(exitDialog.get(), .5));
			//new TransitionEffects::SplitTransition(exitDialog.get(), Globals::WINDOW_WIDTH));
			//new TransitionEffects::BlindsTransition(exitDialog.get(), .5, false, true));
		/*new TransitionEffects::TrueGrowTransition(exitDialog.get(),
		Vector2(.001, .001), Vector2(1, 1)));*/
		/*new TransitionEffects::SlideAndGrowTransition(
		Vector2(-200, -200), exitDialog->getPosition(),
		Vector2(.001, .001), Vector2(1, 1)));*/
		/*new TransitionEffects::GrowTransition(
		Vector2(.0001, 0001), Vector2(1, 1)));*/
		/*new TransitionEffects::SlideTransition(
		Vector2(-200, -200), exitDialog->getPosition()));*/

		//exitDialog->setCloseTransition(
		/*new TransitionEffects::ShrinkTransition(
		Vector2(1, 1), Vector2(.001, .001)));*/
	}



	return true;
}

void MainScreen::reloadGraphicsAssets() {

	for (auto const& control : guiControls)
		control->reloadGraphicsAsset();

	exitDialog->reloadGraphicsAsset();
}

Keyboard::KeyboardStateTracker keyTracker;
void MainScreen::update(double deltaTime) {


	auto state = Keyboard::Get().GetState();
	keyTracker.Update(state);
	if (keyTracker.IsKeyReleased(Keyboard::Escape)) {
		if (exitDialog->isOpen())
			exitDialog->hide();
		else
			exitDialog->show();
	}


	if (exitDialog->isOpen()) {
		exitDialog->update(deltaTime);
	} else {
		for (auto const& control : guiControls)
			control->update(deltaTime);
	}
}


void MainScreen::draw(SpriteBatch* batch) {

	batch->Begin(SpriteSortMode_Deferred);
	{
		for (auto const& control : guiControls)
			control->draw(batch);

		exitDialog->draw(batch);
	}
	batch->End();
}

//void MainScreen::safedraw(SpriteBatch* batch) {
//	for (auto const& control : guiControls)
//		control->draw(batch);
//
//	exitDialog->draw(batch);
//}


void MainScreen::confirmExit() {
	exitDialog->show();
}



/** **** ConfigScreen **** **/
ConfigScreen::ConfigScreen(MenuManager* mngr) : MenuScreen(mngr) {

}

ConfigScreen::~ConfigScreen() {
	guiControls.clear();
}


int MARGIN = 10;
int itemHeight = 32;
bool ConfigScreen::initialize(ComPtr<ID3D11Device> device, shared_ptr<MouseController> mouse) {

	Vector2 controlPos = Vector2(50, 50);
	// Labels for displaying pressed info
	adapterLabel =
		guiFactory->createTextLabel(controlPos, L"Test");
	adapterLabel->setHoverable(true);
	guiControls.push_back(unique_ptr<GUIControl>(adapterLabel));

	controlPos.y += adapterLabel->getHeight() + MARGIN;

	// create listbox of gfx cards
	adapterListbox =
		guiFactory->createListBox(controlPos, 400, itemHeight);
	guiControls.push_back(unique_ptr<GUIControl>(adapterListbox));
	vector<ListItem*> adapterItems;
	for (ComPtr<IDXGIAdapter> adap : game->getAdapterList()) {
		AdapterItem* item = new AdapterItem();
		item->adapter = adap;
		adapterItems.push_back(item);
	}
	adapterListbox->addItems(adapterItems);
	adapterListbox->setSelected(game->getSelectedAdapterIndex());

	OnClickListenerAdapterList* onClickAdapterList = new OnClickListenerAdapterList(this);
	adapterListbox->setActionListener(onClickAdapterList);

	adapterLabel->setText(adapterListbox->getSelected()->toString());


	controlPos.y += adapterListbox->getHeight() + MARGIN * 2;
	displayLabel = guiFactory->createTextLabel(controlPos, L"A");
	guiControls.push_back(unique_ptr<GUIControl>(displayLabel));

	controlPos.y += displayLabel->getHeight() + MARGIN;

	// create listbox of monitors available to pressed gfx card
	displayListbox = guiFactory->createListBox(controlPos, 400, itemHeight);
	guiControls.push_back(unique_ptr<GUIControl>(displayListbox));
	// because only the one adapter has displays on my laptop
	// this has to be grab the first (and only) display.
	populateDisplayList(game->getDisplayListFor(0));
	displayListbox->setSelected(game->getSelectedDisplayIndex());

	displayLabel->setText(displayListbox->getSelected()->toString());

	// setup label for Display Mode
	controlPos.x += displayListbox->getWidth() + MARGIN * 2;
	controlPos.y = 50;


	// Setup display mode combobox

	// custom scrollbar for combo list
	ScrollBarDesc scrollBarDesc;
	scrollBarDesc.upButtonImage = "ScrollBar Up Custom";
	scrollBarDesc.upPressedButtonImage = "ScrollBar Up Pressed Custom";
	//scrollBarDesc.downButtonImage = "ScrollBar Down";
	scrollBarDesc.trackImage = "ScrollBar Track Custom";
	scrollBarDesc.scrubberImage = "Scrubber Custom";

	displayModeCombobox =
		guiFactory->createComboBox(controlPos, 75, itemHeight, 10, true);

	populateDisplayModeList(
		game->getDisplayModeList(0));
	displayModeCombobox->setScrollBar(scrollBarDesc);
	displayModeCombobox->setSelected(game->getSelectedDisplayModeIndex());
	guiControls.push_back(unique_ptr<GUIControl>(displayModeCombobox));

	OnClickListenerDisplayModeList* onClickDisplayMode =
		new OnClickListenerDisplayModeList(this);
	displayModeCombobox->setActionListener(onClickDisplayMode);


	// Set up CheckBox for full-screen toggle
	CheckBox* check = guiFactory->createCheckBox(
		Vector2(50, 450), L"Full Screen");
	OnClickListenerFullScreenCheckBox* onClickFullScreen
		= new OnClickListenerFullScreenCheckBox(this);
	check->setChecked(Globals::FULL_SCREEN);
	check->setActionListener(onClickFullScreen);
	guiControls.push_back(unique_ptr<GUIControl>(check));

	testLabel = guiFactory->createTextLabel(
		Vector2(250, 450), L"Test Messages here");
	guiControls.push_back(unique_ptr<GUIControl>(testLabel));

	// Create Apply and Cancel Buttons
	ImageButton* button = (ImageButton*) guiFactory->
		createImageButton("Button Up", "Button Down");
	button->setText(L"Back");
	button->setPosition(
		Vector2(Globals::WINDOW_WIDTH / 2 - button->getWidth(),
			Globals::WINDOW_HEIGHT - button->getHeight() - 25));
	guiControls.push_back(unique_ptr<GUIControl>(button));

	button = (ImageButton*) guiFactory->
		createImageButton("Button Up", "Button Down");
	button->setText(L"Apply");
	button->setPosition(
		Vector2(Globals::WINDOW_WIDTH / 2,
			Globals::WINDOW_HEIGHT - button->getHeight() - 25));
	guiControls.push_back(unique_ptr<GUIControl>(button));


	return true;
}

void ConfigScreen::reloadGraphicsAssets() {
	for (auto const& control : guiControls)
		control->reloadGraphicsAsset();

}


void ConfigScreen::update(double deltaTime) {

	auto state = Keyboard::Get().GetState();
	keyTracker.Update(state);

	if (keyTracker.IsKeyReleased(Keyboard::Escape)) {
		menuManager->openMainMenu();
	}


	for (auto const& control : guiControls) {
		control->update(deltaTime);
	}
}


void ConfigScreen::draw(SpriteBatch* batch) {
	batch->Begin(SpriteSortMode_Deferred);
	{
		for (auto const& control : guiControls)
			control->draw(batch);
	}
	batch->End();
}

//void ConfigScreen::safedraw(SpriteBatch* batch) {
//	for (auto const& control : guiControls)
//		control->draw(batch);
//}

void ConfigScreen::populateDisplayList(vector<ComPtr<IDXGIOutput>> displays) {

	displayListbox->clear();
	vector<ListItem*> adapterOuts;
	for (ComPtr<IDXGIOutput> adap : displays) {

		DisplayItem* item = new DisplayItem();
		item->adapterOutput = adap;
		adapterOuts.push_back(item);
	}
	displayListbox->addItems(adapterOuts);
}

void ConfigScreen::populateDisplayModeList(vector<DXGI_MODE_DESC> displayModes) {

	displayModeCombobox->clear();
	vector<ListItem*> displayModeItems;
	for (DXGI_MODE_DESC mode : displayModes) {
		/*if (!Globals::FULL_SCREEN && mode.Scaling == 0)
		continue;*/
		DisplayModeItem* item = new DisplayModeItem();
		item->modeDesc = mode;
		displayModeItems.push_back(item);
	}
	displayModeCombobox->addItems(displayModeItems);
}


void AdapterItem::setText() {

	DXGI_ADAPTER_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_ADAPTER_DESC));
	adapter->GetDesc(&desc);
	wostringstream wss;
	if (isEnumerated) {
		wss << listPosition << ": ";
	}
	wss << desc.Description;
	textLabel->setText(wss);
}


void DisplayItem::setText() {

	DXGI_OUTPUT_DESC desc;
	adapterOutput->GetDesc(&desc);
	wostringstream wss;
	if (isEnumerated) {
		wss << listPosition << ": ";
	}
	wss << desc.DeviceName;
	textLabel->setText(wss);
}

void DisplayModeItem::setText() {

	UINT width = modeDesc.Width;
	UINT height = modeDesc.Height;

	wostringstream wss;
	if (isEnumerated) {
		wss << listPosition << ": ";
	}
	wss << width << " x " << height;
	wss << " Fill: " << ((modeDesc.Scaling == 0) ? L"Yes" : L"No");
	textLabel->setText(wss.str());

}



void OnClickListenerAdapterList::onClick(ListBox* listbox, UINT selectedIndex) {

	AdapterItem* selectedItem = (AdapterItem*) listbox->getItem(selectedIndex);
	config->game->setAdapter(selectedIndex);
	config->populateDisplayList(config->game->getDisplayList());
	config->populateDisplayModeList(
		config->game->getDisplayModeList(0
		/*config->game->getSelectedAdapterIndex()*/));

	config->adapterLabel->setText(listbox->getSelected()->toString());
}


void OnClickListenerAdapterList::onHover(ListBox* listbox, short hoveredItemIndex) {
}



void OnClickListenerDisplayModeList::onClick(ComboBox* combobox, UINT selectedIndex) {

	if (!config->game->setDisplayMode(selectedIndex)) {
		// change back to previous setting
		config->testLabel->setText("Display mode switch failed!");
	} else {
		// reconstruct display
		config->testLabel->setText(combobox->getItem(selectedIndex)->toString());
	}

}

void OnClickListenerDisplayModeList::onHover(ComboBox * listbox, short hoveredItemIndex) {
}



void OnClickListenerFullScreenCheckBox::onClick(CheckBox* checkbox, bool isChecked) {

	/*wostringstream wss;
	wss << "CheckBox is checked: " << isChecked;
	config->displayModeLabel->setText(wss);*/
	if (!config->game->setFullScreen(isChecked)) {
		// revert to old settings
	} else {
		// reconstruct display

		config->populateDisplayModeList(config->game->getDisplayModeList(0));
		//config->displayModeCombobox->setSelected(config->game->getSelectedDisplayModeIndex());
	}
}

void OnClickListenerFullScreenCheckBox::onHover(CheckBox* checkbox,
	bool isChecked) {
}



void SettingsButtonListener::onClick(Button* button) {

	main->menuManager->openConfigMenu();
}

void SettingsButtonListener::onPress(Button * button) {
}

void SettingsButtonListener::onHover(Button * button) {
}

void SettingsButtonListener::resetState(Button * button) {
}

void OnClickListenerDialogQuitButton::onClick(Button* button) {
	main->exitDialog->hide();
	main->game->exit();
}

void OnClickListenerDialogQuitButton::onPress(Button * button) {
}

void OnClickListenerDialogQuitButton::onHover(Button * button) {
}

void OnClickListenerDialogQuitButton::resetState(Button * button) {
}

void OnClickListenerExitButton::onClick(Button* button) {
	main->exitDialog->show();
}

void OnClickListenerExitButton::onPress(Button * button) {
}

void OnClickListenerExitButton::onHover(Button * button) {
}

void OnClickListenerExitButton::resetState(Button * button) {
}


void PlayButtonListener::onClick(Button* button) {

	main->menuManager->openLevelSelectScreen();
}

void PlayButtonListener::onPress(Button * button) {
}

void PlayButtonListener::onHover(Button * button) {
}

void PlayButtonListener::resetState(Button * button) {
}
