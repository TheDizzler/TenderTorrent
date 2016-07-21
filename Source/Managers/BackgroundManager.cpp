#include "BackgroundManager.h"

BackgroundManager::BackgroundManager() {
}

BackgroundManager::~BackgroundManager() {
}


bool BackgroundManager::loadLevel(ID3D11Device* device, const wchar_t* file) {

	bg.reset(new Background());
	if (!bg->load(device, file))
		return false;

	return true;
}

void BackgroundManager::clear() {

	bg->clear();
}

//void BackgroundManager::startUpdate(double deltaTime) {
//}

void BackgroundManager::update(double deltaTime, PlayerShip* player) {

	bg->update(deltaTime, player);
}

void BackgroundManager::draw(SpriteBatch * batch) {

	bg->draw(batch);
}

vector<BackgroundLayer*> BackgroundManager::getLayers() {

	return bg->bgLayers;
}
