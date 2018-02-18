#include "../pch.h"
#include "GameManager.h"
#include "../assets.h"
#include "../Engine/GameEngine.h"
#include "../../DXTKGui/StringHelper.h"

GUIFactory guiFactory;
unique_ptr<GFXAssetManager> gfxAssets;

unique_ptr<PromptDialog> GameManager::errorDialog;
unique_ptr<PromptDialog> GameManager::warningDialog;
Dialog* GameManager::showDialog = NULL;


GameManager::~GameManager() {
	delete blendState;
	showDialog = NULL;
}

bool GameManager::initializeGame(GameEngine* gmngn,
	HWND hwnd, ComPtr<ID3D11Device> dvc) {

	gameEngine = gmngn;

	device = dvc;


	// get graphical assets from xml file
	docAssMan.reset(new pugi::xml_document());
	if (!docAssMan->load_file(Assets::assetManifestFile)) {
		StringHelper::reportError(L"Could not read AssetManifest file!",
			L"Fatal Read Error!", !Globals::FULL_SCREEN);
		return false;
	}

	//xml_node guiAssetsNode = docAssMan->child("root").child("gui");
	xml_node gfxAssetNode = docAssMan->child("root").child("gfx");
	gfxAssets.reset(new GFXAssetManager(gfxAssetNode));
	if (!gfxAssets->initialize(device)) {
		StringHelper::reportError(L"Failed to load GFX Assets.", L"Fatal Error",
			!Globals::FULL_SCREEN);
		return false;
	}


	if (!guiFactory.initialize(hwnd, device, gameEngine->getDeviceContext(),
		gameEngine->getSwapChain(), gameEngine->getSpriteBatch(), &mouse,
		Assets::assetManifestFile)) {
		StringHelper::reportError(L"Failed to load GUIFactory", L"Fatal Error",
			!Globals::FULL_SCREEN);
		return false;
	}

	mouse.loadMouseIcon(&guiFactory, "Mouse Arrow");
	ShowCursor(false);

	blendState = new CommonStates(device.Get());

	initErrorDialogs();



	menuScreen.reset(new MenuManager());
	menuScreen->setGameManager(this);
	if (!menuScreen->initialize(device))
		return false;


	levelScreen.reset(new LevelManager());
	levelScreen->setGameManager(this);
	if (!levelScreen->initialize(device))
		return false;


	currentScreen = menuScreen.get();

	/*transitionManager.reset(
		new ScreenTransitions::ScreenTransitionManager(
			guiFactory.get(), "Default Transition BG"));*/
	//transitionManager = make_unique<ScreenTransitions::ScreenTransitionManager>();
	transitionManager.initialize(&guiFactory, "Default Transition BG");
	transitionManager.setTransition(
		//new ScreenTransitions::FlipScreenTransition(true));
		new ScreenTransitions::SquareFlipScreenTransition());
	//new ScreenTransitions::LineWipeScreenTransition());

	return true;
}

void GameManager::reloadGraphicsAssets() {

	blendState = new CommonStates(device.Get());
	errorDialog->reloadGraphicsAsset();
	warningDialog->reloadGraphicsAsset();

	transitionManager.reloadGraphicsAssets();
	//levelScreen->reloadGraphicsAssets();
	menuScreen->reloadGraphicsAssets();
	mouse.reloadGraphicsAsset(&guiFactory);
}



void GameManager::initErrorDialogs() {

	Vector2 dialogPos, dialogSize;
	dialogSize = Vector2((float) Globals::WINDOW_WIDTH / 2, (float) Globals::WINDOW_HEIGHT / 2);
	dialogPos = dialogSize;
	dialogPos.x -= dialogSize.x / 2;
	dialogPos.y -= dialogSize.y / 2;
	errorDialog.reset(guiFactory.createDialog(dialogSize, dialogPos, false, true, 5));
	errorDialog->setTint(Color(0, 120, 207));
	unique_ptr<Button> quitButton;
	quitButton.reset(guiFactory.createButton());
	quitButton->setText(L"Exit Program");
	quitButton->setActionListener(new QuitButtonListener(this));
	//quitButton->setMatrixFunction([&]()-> Matrix { return camera->translationMatrix(); });
	errorDialog->setCancelButton(move(quitButton));

	ScrollBarDesc scrollBarDesc;
	scrollBarDesc.upButtonImage = "ScrollBar Up Custom";
	scrollBarDesc.upPressedButtonImage = "ScrollBar Up Pressed Custom";
	scrollBarDesc.trackImage = "ScrollBar Track Custom";
	scrollBarDesc.scrubberImage = "Scrubber Custom";
	warningDialog.reset(guiFactory.createDialog(dialogPos, dialogSize, false, true, 3));
	warningDialog->setScrollBar(scrollBarDesc);
	//warningDialog->setMatrixFunction([&]()-> Matrix { return camera->translationMatrix(); });
	warningDialog->setTint(Color(0, 120, 207));
	warningDialog->setCancelButton(L"Continue");
	unique_ptr<Button> quitButton2;
	quitButton2.reset(guiFactory.createButton());
	quitButton2->setText(L"Exit Program");
	quitButton2->setActionListener(new QuitButtonListener(this));
	warningDialog->setConfirmButton(move(quitButton2));

	showDialog = warningDialog.get();
}


void GameManager::update(double deltaTime) {

	if (switchTo != NULL) {

		if (transitionManager.runTransition(deltaTime)) {
			currentScreen = switchTo;
			switchTo = NULL;
		}

	} else
		currentScreen->update(deltaTime);
}


void GameManager::draw(SpriteBatch* batch) {
	if (switchTo != NULL) {
		batch->Begin(SpriteSortMode_Deferred);
		{
			transitionManager.drawTransition(batch);
		}
		batch->End();
	} else {
		/*batch->Begin(SpriteSortMode_Deferred, NULL, NULL, NULL, NULL, NULL,
			camera->translationMatrix());
		{*/
		currentScreen->draw(batch);
	/*}
	batch->End();*/
	}

	batch->Begin(SpriteSortMode_Deferred);
	{
		mouse.draw(batch);
	}
	batch->End();
}

void GameManager::loadLevel(string levelName) {

	if (!levelScreen->loadLevel(device, levelName.c_str())) {
		wostringstream msg;
		msg << "Failed to load level: " << levelName.c_str();
		GameEngine::showErrorDialog(msg.str(), L"Fatal Error");
		return;
	}
	switchTo = levelScreen.get();
	transitionManager.transitionBetween(currentScreen, switchTo);
	lastScreen = currentScreen;
	//currentScreen = levelScreen.get();


}

void GameManager::loadMainMenu() {

	switchTo = menuScreen.get();
	transitionManager.transitionBetween(currentScreen, switchTo);
	lastScreen = currentScreen;
	//currentScreen = menuScreen.get();

}


void GameManager::pause() {

	if (currentScreen != NULL)
		currentScreen->pause();
}

void GameManager::exit() {
	gameEngine->exit();
}

void GameManager::controllerRemoved(ControllerSocketNumber controllerSocket,
	PlayerSlotNumber slotNumber) {
	currentScreen->controllerRemoved(controllerSocket, slotNumber);
}

void GameManager::newController(shared_ptr<Joystick> newStick) {
	currentScreen->newController(newStick);
}


vector<ComPtr<IDXGIAdapter>> GameManager::getAdapterList() {
	return gameEngine->getAdapterList();
}

vector<ComPtr<IDXGIOutput>> GameManager::getDisplayList() {
	return gameEngine->getDisplayList();
}

vector<ComPtr<IDXGIOutput>> GameManager::getDisplayListFor(size_t adapterIndex) {
	return gameEngine->getDisplayListFor(adapterIndex);
}

vector<ComPtr<IDXGIOutput>> GameManager::getDisplayListFor(
	ComPtr<IDXGIAdapter> adapter) {
	return gameEngine->getDisplayListFor(adapter);
}

vector<DXGI_MODE_DESC> GameManager::getDisplayModeList(size_t displayIndex) {
	return gameEngine->getDisplayModeList(displayIndex);
}

vector<DXGI_MODE_DESC> GameManager::getDisplayModeList(ComPtr<IDXGIOutput> display) {
	return gameEngine->getDisplayModeList(display);
}

bool GameManager::setAdapter(size_t adapterIndex) {
	return gameEngine->setAdapter(adapterIndex);
}

bool GameManager::setDisplayMode(size_t displayModeIndex) {
	return gameEngine->changeDisplayMode(displayModeIndex);
}

bool GameManager::setFullScreen(bool isFullScreen) {
	return gameEngine->setFullScreen(isFullScreen);
}


size_t GameManager::getSelectedAdapterIndex() {
	return gameEngine->getSelectedAdapterIndex();
}

size_t GameManager::getSelectedDisplayIndex() {
	return gameEngine->getSelectedDisplayIndex();
}

size_t GameManager::getSelectedDisplayModeIndex() {
	return gameEngine->getSelectedDisplayModeIndex();
}






