#include "GameManager.h"

unique_ptr<GUIFactory> GameManager::guiFactory;
unique_ptr<GFXAssetManager> GameManager::gfxAssets;

GameManager::GameManager(GameEngine* gmngn) {
	gameEngine = gmngn;
}

GameManager::~GameManager() {
}

#include "../DXTKGui/GuiAssets.h"
#include "../Engine/GameEngine.h"
bool GameManager::initializeGame(HWND hwnd, ComPtr<ID3D11Device> dvc, MouseController* ms) {

	device = dvc;
	mouse = ms;

	// get graphical assets from xml file
	docAssMan.reset(new pugi::xml_document());
	if (!docAssMan->load_file(GUIAssets::assetManifestFile)) {
		MessageBox(0, L"Could not read AssetManifest file!",
			L"Fatal Read Error!", MB_OK);
		return false;
	}

	xml_node guiAssetsNode = docAssMan->child("root").child("gui");
	xml_node gfxAssetNode = docAssMan->child("root").child("gfx");
	gfxAssets.reset(new GFXAssetManager(gfxAssetNode));
	if (!gfxAssets->initialize(device)) {
		MessageBox(0, L"Failed to load GFX Assets.", L"Fatal Error", MB_OK);
		return false;
	}

	guiFactory.reset(new GUIFactory(hwnd, guiAssetsNode));
	if (!guiFactory->initialize(device, gameEngine->getDeviceContext(),
		gameEngine->getSwapChain(), gameEngine->getSpriteBatch())) {

		MessageBox(0, L"Failed to load GUIFactory", L"Fatal Error", MB_OK);
		return false;
	}
	gameEngine->constructErrorDialogs();

	

	menuScreen.reset(new MenuManager());
	menuScreen->setGameManager(this);
	if (!menuScreen->initialize(device, mouse))
		return false;


	levelScreen.reset(new LevelManager());
	levelScreen->setGameManager(this);
	if (!levelScreen->initialize(device, mouse))
		return false;
	

	currentScreen = menuScreen.get();

	transitionManager.reset(
		new ScreenTransitions::ScreenTransitionManager(
			GameManager::guiFactory.get(), "Default Transition BG"));
	transitionManager->setTransition(
		//new ScreenTransitions::FlipScreenTransition(true));
		new ScreenTransitions::SquareFlipScreenTransition());
		//new ScreenTransitions::LineWipeScreenTransition());

	return true;
}


void GameManager::update(double deltaTime, KeyboardController* keys,
	MouseController* mouse) {
	
	if (switchTo != NULL) {
		if (transitionManager->runTransition(deltaTime)) {
			currentScreen = switchTo;
			switchTo = NULL;
		}
	} else
		currentScreen->update(deltaTime, keys, mouse);
}


void GameManager::draw(SpriteBatch * batch) {
	if (switchTo != NULL) {
		transitionManager->drawTransition(batch);
	} else
		currentScreen->draw(batch);
}

void GameManager::loadLevel(string levelName) {

	if (!levelScreen->loadLevel(device, levelName.c_str())) {
		wostringstream msg;
		msg << "Failed to load level: " << levelName.c_str();
		GameEngine::showErrorDialog(msg.str(), L"Fatal Error");
		return;
	}
	switchTo = levelScreen.get();
	transitionManager->transitionBetween(currentScreen, switchTo);
	lastScreen = currentScreen;
	//currentScreen = levelScreen.get();

	
}

void GameManager::loadMainMenu() {

	switchTo = menuScreen.get();
	transitionManager->transitionBetween(currentScreen, switchTo);
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

vector<ComPtr<IDXGIAdapter>> GameManager::getAdapterList() {
	return gameEngine->getAdapterList();
}

vector<ComPtr<IDXGIOutput>> GameManager::getDisplayList() {
	return gameEngine->getDisplayList();
}

vector<ComPtr<IDXGIOutput>> GameManager::getDisplayListFor(size_t displayIndex) {
	return gameEngine->getDisplayListFor(displayIndex);
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






