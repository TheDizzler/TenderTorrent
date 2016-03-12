#include "GameManager.h"

GameManager::GameManager(GameEngine* gmngn) {

	gameEngine = gmngn;
}

GameManager::~GameManager() {
}

bool GameManager::initializeGame(ID3D11Device* dvc, MouseController* ms) {

	device = dvc;
	mouse = ms;

	//currentScreen.reset(new MenuManager());
	currentScreen = new MenuManager();
	if (!currentScreen->initialize(device, mouse))
		return false;
	currentScreen->setGameManager(this);

	return true;
}

void GameManager::update(double deltaTime, BYTE keyboardState[256],
	MouseController* mouse) {

	currentScreen->update(deltaTime, keyboardState, mouse);
}

void GameManager::draw(SpriteBatch * batch) {

	currentScreen->draw(batch);
}

void GameManager::loadLevel() {

	//currentScreen.reset(new LevelManager());
	if (lastScreen)
		delete lastScreen;
	lastScreen = currentScreen;
	currentScreen = new LevelManager();

	currentScreen->initialize(device, mouse);
	currentScreen->setGameManager(this);

}

void GameManager::loadMainMenu() {

	if (lastScreen)
		delete lastScreen;
	lastScreen = currentScreen;
	currentScreen = new MenuManager();

	currentScreen->initialize(device, mouse);
	currentScreen->setGameManager(this);
}


#include "../Engine/GameEngine.h"

void GameManager::quit() {

	gameEngine->quit();
}

