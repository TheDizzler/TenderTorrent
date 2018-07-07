#include "../pch.h"
#include "MenuManager.h"
#include "GameManager.h"
#include "../Engine/GameEngine.h"

MenuManager::MenuManager() {
}

MenuManager::~MenuManager() {
}

void MenuManager::setGameManager(GameManager* gm) {
	game = gm;
}

void MenuManager::reloadGraphicsAssets() {

	transitionManager.reloadGraphicsAssets();
	mainScreen->reloadGraphicsAssets();
	configScreen->reloadGraphicsAssets();
}


bool MenuManager::initialize(ComPtr<ID3D11Device> device) {


	if (!mouse.loadMouseIcon(&guiFactory, "Mouse Reticle"))
		return false;

	mainScreen.reset(new MainScreen(this));
	mainScreen->setGameManager(game);
	if (!mainScreen->initialize(device))
		return false;

	configScreen.reset(new ConfigScreen(this));
	configScreen->setGameManager(game);
	if (!configScreen->initialize(device))
		return false;
	configScreen->update(0);

	levelSelectScreen.reset(new LevelSelectScreen(this));
	if (!levelSelectScreen->initialize(device)) {
		return false;
	};

#ifdef _QUICK_LAUNCH
	currentScreen = levelSelectScreen.get();
#else
	currentScreen = mainScreen.get();
#endif

	/*transitionManager.reset(
		new ScreenTransitions::ScreenTransitionManager(
			guiFactory.get(), "Default Transition BG"));*/
	transitionManager.initialize(&guiFactory, "Default Transition BG");
	transitionManager.setTransition(
		//new ScreenTransitions::FlipScreenTransition(true));
		//new ScreenTransitions::SquareFlipScreenTransition());
		new ScreenTransitions::LineWipeScreenTransition());

	return true;
}


void MenuManager::update(double deltaTime) {

	if (switchTo != NULL) {
		if (transitionManager.runTransition(deltaTime)) {
			currentScreen = switchTo;
			switchTo = NULL;
		}
	} else
		currentScreen->update(deltaTime);

}


void MenuManager::draw(SpriteBatch* batch) {


	if (switchTo != NULL) {
		batch->Begin(SpriteSortMode_Deferred);
		{
			transitionManager.drawTransition(batch);
		}
		batch->End();
	} else {
		batch->Begin(SpriteSortMode_Deferred);
		{
			currentScreen->draw(batch);
		}
		batch->End();
	}
}

void MenuManager::textureDraw(SpriteBatch* batch) {
	batch->Begin(SpriteSortMode_Deferred);
	{
		currentScreen->draw(batch);
	}
	batch->End();
}


void MenuManager::pause() {
	// do nothing?
}

void MenuManager::openMainMenu() {

	switchTo = mainScreen.get();
	transitionManager.transitionBetween(currentScreen, switchTo);
}

void MenuManager::openConfigMenu() {
	configScreen->setup();
	switchTo = configScreen.get();
	transitionManager.transitionBetween(currentScreen, switchTo);
}

void MenuManager::openLevelSelectScreen() {

	switchTo = levelSelectScreen.get();
	transitionManager.transitionBetween(currentScreen, switchTo);
}

void MenuManager::loadLevel(string levelXMLFile) {

	game->loadLevel(levelXMLFile);
}

//void MenuManager::notifyControllerRemoved(ControllerSocketNumber controllerSlot, PlayerSlotNumber slotNumber) {
//	if (currentScreen != configScreen.get()) {
//		configScreen->controllerRemoved(controllerSlot, slotNumber);
//	}
//}

void MenuManager::controllerRemoved(ControllerSocketNumber controllerSlot, PlayerSlotNumber slotNumber) {
	currentScreen->controllerRemoved(controllerSlot, slotNumber);
}

void MenuManager::newController(shared_ptr<Joystick> newStick) {
	currentScreen->newController(newStick);
}




/** **** MenuScreen abstract class **** */
MenuScreen::MenuScreen(MenuManager* mngr) {

	menuManager = mngr;
}

MenuScreen::~MenuScreen() {

	for (GUIControl* control : guiControls)
		delete control;

	//guiControls.clear();
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


bool MainScreen::initialize(ComPtr<ID3D11Device> device) {

	Vector2 buttonpos = Vector2(
		float(Globals::WINDOW_WIDTH) / 2, float(Globals::WINDOW_HEIGHT) / 8);
	Vector2 buttonsize = Vector2(float(Globals::WINDOW_WIDTH) / 4, 0);
	buttonpos.x -= buttonsize.x / 2;
	Button* button = guiFactory.createButton(buttonpos, buttonsize, L"Play");
	button->setUnpressedColor(Color(1, .558f, 1, 1));
	button->setActionListener(new PlayButtonListener(this));
	LetterJammer* jammer = guiFactory.createLetterJammer(
		Vector2::Zero, L"Play", Color(0, 0, 0, 1), false);
	jammer->setEffect(make_unique<ColorJammer>(.525f));
	button->setTextLabel(jammer, true);
	guiControls.push_back(button);

	buttonpos.y = float(Globals::WINDOW_HEIGHT) / 2 - float(button->getHeight()) / 2;
	button = guiFactory.createButton(buttonpos, buttonsize, L"Settings");
	button->setUnpressedColor(Color(1, .558f, 1, 1));
	button->setActionListener(new SettingsButtonListener(this));
	guiControls.push_back(button);

	buttonpos.y = float(Globals::WINDOW_HEIGHT - Globals::WINDOW_HEIGHT / 8 - button->getHeight());
	button = guiFactory.createButton(buttonpos, buttonsize, L"Exit");
	button->setUnpressedColor(Color(1, .558f, 1, 1));
	button->setActionListener(new OnClickListenerExitButton(this));
	guiControls.push_back(button);

	/*delete*/

	{

		Vector2 dialogPos, dialogSize;
		dialogSize = Vector2(float(Globals::WINDOW_WIDTH) / 2, float(Globals::WINDOW_HEIGHT) / 2);
		dialogPos = dialogSize;
		dialogPos.x -= dialogSize.x / 2;
		dialogPos.y -= dialogSize.y / 2;
		exitDialog.reset(guiFactory.createDialog(dialogPos, dialogSize, true, true, 10));
		exitDialog->setTint(Color(0, .5, 1, 1));
		exitDialog->setTitle(L"Exit Test?", Vector2(1, 1), "BlackCloak");
		//exitDialog->setTitleAreaDimensions(Vector2(0, 150));
		exitDialog->setText(L"Really Quit The Test Project?");
		unique_ptr<Button> quitButton;
		quitButton.reset(guiFactory.createButton());
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


void MainScreen::update(double deltaTime) {

	if (keys.isKeyPressed(Keyboard::Escape)) {
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
	for (auto const& control : guiControls)
		control->draw(batch);

	exitDialog->draw(batch);
}


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
bool ConfigScreen::initialize(ComPtr<ID3D11Device> device) {


	Vector2 controlPos = Vector2(50, 50);
	//Labels for displaying pressed info
	adapterLabel = guiFactory.createTextLabel(controlPos, L"Test");
	adapterLabel->setHoverable(true);
	guiControls.push_back(adapterLabel);

	controlPos.y += adapterLabel->getHeight() + MARGIN;

	// create listbox of gfx cards
	adapterListbox = guiFactory.createListBox(controlPos, 400, itemHeight);
	guiControls.push_back(adapterListbox);
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
	displayLabel = guiFactory.createTextLabel(controlPos, L"A");
	guiControls.push_back(displayLabel);

	controlPos.y += displayLabel->getHeight() + MARGIN;

	// create listbox of monitors available to pressed gfx card
	displayListbox = guiFactory.createListBox(controlPos, 400, itemHeight);
	guiControls.push_back(displayListbox);
	// because only the one adapter has displays on my laptop
	// this has to be grab the first (and only) display.
	populateDisplayList(game->getDisplayListFor(0));
	displayListbox->setSelected(game->getSelectedDisplayIndex());
	displayListbox->setActionListener(new OnClickListenerDisplayList(this));

	displayLabel->setText(displayListbox->getSelected()->toString());
	displayLabel->setHoverable(true);
	// setup label for Display Mode

	controlPos.y += displayListbox->getHeight() + 50;

	testSpinner = guiFactory.createSpinner(controlPos, 25, itemHeight);

	vector<wstring> items;
	for (const auto& playerSlot : waitingSlots) {
		wostringstream wss;
		wss << "Slot: " << playerSlot->getPlayerSlotNumber();
		items.push_back(wss.str());
	}

	testSpinner->addItems(items);
	guiControls.push_back(testSpinner);
	controlPos.x += testSpinner->getWidth() + 25;

	spinnerLabel = guiFactory.createTextLabel(controlPos, L"Controllers Found");
	guiControls.push_back(spinnerLabel);


	//Setup display mode combobox
	controlPos.x = displayListbox->getPosition().x + displayListbox->getWidth() + MARGIN * 2;
	controlPos.y = 50;
	//custom scrollbar for combo list
	ScrollBarDesc scrollBarDesc;
	scrollBarDesc.upButtonImage = "ScrollBar Up Custom";
	scrollBarDesc.upPressedButtonImage = "ScrollBar Up Pressed Custom";
	//scrollBarDesc.downButtonImage = "ScrollBar Down";
	scrollBarDesc.trackImage = "ScrollBar Track Custom";
	scrollBarDesc.scrubberImage = "Scrubber Custom";

	displayModeCombobox =
		guiFactory.createComboBox(controlPos, 75, itemHeight, 10, true);
	//guiFactory->createListBox(controlPos, 75, itemHeight);

	populateDisplayModeList(game->getDisplayModeList(displayListbox->getSelectedIndex()));
	displayModeCombobox->setScrollBar(scrollBarDesc);
	displayModeCombobox->setSelected(game->getSelectedDisplayModeIndex());
	OnClickListenerDisplayModeList* onClickDisplayMode =
		new OnClickListenerDisplayModeList(this);
	displayModeCombobox->setActionListener(onClickDisplayMode);
	guiControls.push_back(displayModeCombobox);




	// Set up CheckBox for full-screen toggle
	CheckBox* check = guiFactory.createCheckBox(Vector2(50, 450), L"Full Screen");
	OnClickListenerFullScreenCheckBox* onClickFullScreen
		= new OnClickListenerFullScreenCheckBox(this);
	check->setActionListener(onClickFullScreen);
	check->setChecked(Globals::FULL_SCREEN);

	guiControls.push_back(check);

	testLabel = guiFactory.createTextLabel(
		Vector2(250, 450), L"Test Messages here");
	guiControls.push_back(testLabel);

	// Create Apply and Cancel Buttons
	ImageButton* button = (ImageButton*) guiFactory.createImageButton("Button Up", "Button Down");
	button->setText(L"Back");
	button->setPosition(
		Vector2((float) Globals::WINDOW_WIDTH / 2 - button->getWidth(),
		(float) Globals::WINDOW_HEIGHT - button->getHeight() - 25));
	button->setActionListener(new BackButtonListener(this));
	guiControls.push_back(button);

	button = (ImageButton*) guiFactory.createImageButton("Button Up", "Button Down");
	button->setText(L"Apply");
	button->setPosition(
		Vector2((float) Globals::WINDOW_WIDTH / 2,
		(float) Globals::WINDOW_HEIGHT - button->getHeight() - 25));
	guiControls.push_back(button);

	texturePanel.reset(guiFactory.createPanel());


	return true;
}

void ConfigScreen::reloadGraphicsAssets() {
	for (auto const& control : guiControls)
		control->reloadGraphicsAsset();
}

void ConfigScreen::setup() {

	testSpinner->clear();
	vector<wstring> items;
	for (const auto& playerSlot : activeSlots) {
		wostringstream wss;
		wss << "Socket: " << playerSlot->getStick()->getControllerSockerNumber();
		wss << " Player: " << playerSlot->getPlayerSlotNumber();
		items.push_back(wss.str());
	}
	testSpinner->addItems(items);

	Vector2 newPos(testSpinner->getPosition());
	newPos.x += testSpinner->getWidth() + 25;
	spinnerLabel->setPosition(newPos);
}


void ConfigScreen::update(double deltaTime) {


	if (keys.isKeyPressed(Keyboard::Escape)) {
		menuManager->openMainMenu();
	}

	for (auto const& control : guiControls) {
		if (control->update(deltaTime))
			refreshTexture = true;
	}

	if (refreshTexture) {
		refreshTexture = false;
		texturePanel->setTexture(guiFactory.createTextureFromScreen(this));
	}
}


void ConfigScreen::draw(SpriteBatch* batch) {
	texturePanel->draw(batch);

}

void ConfigScreen::textureDraw(SpriteBatch* batch) {
	for (auto const& control : guiControls)
		control->draw(batch);
}


void ConfigScreen::controllerRemoved(ControllerSocketNumber controllerSlot,
	PlayerSlotNumber slotNumber) {
	wostringstream wss;
	wss << "Socket: " << controllerSlot;
	wss << " Player: " << slotNumber;
	if (!testSpinner->removeItem(wss.str())) {
		GameEngine::showWarningDialog(
			L"Joystick not found in spinner", L"Joystick Removal Error");
	}
	refreshTexture = true;
}

void ConfigScreen::newController(shared_ptr<Joystick> newStick) {
	wostringstream wss;
	wss << "Socket: " << newStick->getControllerSockerNumber();
	wss << " Player: " << newStick->getPlayerSlotNumber();
	testSpinner->addItem(wss.str());

	Vector2 newPos(testSpinner->getPosition());
	newPos.x += testSpinner->getWidth() + 25;
	spinnerLabel->setPosition(newPos);
	refreshTexture = true;
}

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

	displayModeItems.clear();
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


void OnClickListenerDisplayList::onClick(ListBox* listbox, size_t selectedItemIndex) {
	DisplayItem* displayItem = (DisplayItem*) listbox->getItem(selectedItemIndex);
	//config->game->setDisplay(selectedIndex);
	config->populateDisplayModeList(config->game->getDisplayModeList(selectedItemIndex));
	config->displayLabel->setText(displayItem->toString());
}

void OnClickListenerDisplayList::onHover(ListBox* listbox, int hoveredItemIndex) {
}


void OnClickListenerAdapterList::onClick(ListBox* listbox, size_t selectedIndex) {

	//AdapterItem* selectedItem = (AdapterItem*) listbox->getItem(selectedIndex);
	config->game->setAdapter(selectedIndex);
}


void OnClickListenerAdapterList::onHover(ListBox* listbox, int hoveredItemIndex) {
}



void OnClickListenerDisplayModeList::onClick(ComboBox* combobox, size_t selectedIndex) {

	if (!config->game->setDisplayMode(selectedIndex)) {
		// change back to previous setting
		config->testLabel->setText("Display mode switch failed!");
	} else {
		// reconstruct display
		config->testLabel->setText(combobox->getItem(selectedIndex)->toString());
	}

}

void OnClickListenerDisplayModeList::onHover(ComboBox* listbox, int hoveredItemIndex) {
}



void OnClickListenerFullScreenCheckBox::onClick(CheckBox* checkbox, bool isChecked) {

	/*wostringstream wss;
	wss << "CheckBox is checked: " << isChecked;
	config->displayModeLabel->setText(wss);*/
	if (!config->game->setFullScreen(isChecked)) {
		// revert to old settings
	} else {
		// reconstruct display

		config->populateDisplayModeList(
			config->game->getDisplayModeList(config->game->getSelectedDisplayIndex()));
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

void OnClickListenerDialogQuitButton::resetState(Button* button) {
}

void OnClickListenerExitButton::onClick(Button* button) {
	main->exitDialog->show();
}

void OnClickListenerExitButton::onPress(Button* button) {
}

void OnClickListenerExitButton::onHover(Button* button) {
}

void OnClickListenerExitButton::resetState(Button* button) {
}


void PlayButtonListener::onClick(Button* button) {
	main->menuManager->openLevelSelectScreen();
}

void PlayButtonListener::onPress(Button* button) {
}

void PlayButtonListener::onHover(Button* button) {
	((LetterJammer*) button->buttonLabel.get())->setRun(true);
}

void PlayButtonListener::resetState(Button* button) {
	((LetterJammer*) button->buttonLabel.get())->reset();
}

void BackButtonListener::onClick(Button* button) {
	configScreen->menuManager->openMainMenu();
}

void BackButtonListener::onPress(Button* button) {
}

void BackButtonListener::onHover(Button* button) {
}

void BackButtonListener::resetState(Button * button) {
}


