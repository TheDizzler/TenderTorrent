#include "GameManager.h"

GameManager::GameManager() {
}

GameManager::~GameManager() {
}

bool GameManager::initializeGame(ID3D11Device* device, MouseController* mouse) {

	currentScreen.reset(new MenuManager());
	if (!currentScreen->initialize(device, mouse))
		return false;

	return true;
}

void GameManager::update(double deltaTime, BYTE keyboardState[256],
	MouseController* mouse) {

	currentScreen->update(deltaTime, keyboardState, mouse);
}

void GameManager::draw(SpriteBatch * batch) {

	currentScreen->draw(batch);
}
