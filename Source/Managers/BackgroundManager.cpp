#include "BackgroundManager.h"

BackgroundManager::BackgroundManager() {
}

BackgroundManager::~BackgroundManager() {
}

bool BackgroundManager::initialize(ID3D11Device * device) {

	bg.reset(new Background());
	if (!bg->load(device, Assets::bgMakoBGFile))
		return false;

	return true;
}

void BackgroundManager::startUpdate(double deltaTime) {
}

void BackgroundManager::update(double deltaTime, PlayerShip* player) {

	bg->update(deltaTime, player);
}

void BackgroundManager::draw(SpriteBatch * batch) {

	bg->draw(batch);
}

vector<BackgroundLayer*> BackgroundManager::getLayers() {

	return bg->bgLayers;
}
