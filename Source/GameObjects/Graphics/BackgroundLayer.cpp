#include "BackgroundLayer.h"

BackgroundLayer::BackgroundLayer() {
}

BackgroundLayer::~BackgroundLayer() {
}


void BackgroundLayer::update(double deltaTime) {

	//if (topDone) {
	for (const auto& layer : bottomLayers)
		layer->update(deltaTime);
//} else {
	topDone = true;
	for (const auto& layer : topLayers) {
		layer->update(deltaTime);
		if (layer->isAlive())
			topDone = false;
	}
//}
}

void BackgroundLayer::draw(SpriteBatch* batch) {

	for (const auto& layer : bottomLayers)
		layer->draw(batch);
	for (const auto& layer : topLayers)
		layer->draw(batch);
}

void BackgroundLayer::getLayers(vector<unique_ptr<ClothLayer>>& bgLayers) {

	if (topDone)
		bgLayers.insert(bgLayers.end(), bottomLayers.begin(), bottomLayers.end());
	else
		bgLayers.insert(bgLayers.end(), topLayers.begin(), topLayers.end());
}


void BackgroundLayer::addTopLayer(unique_ptr<ClothLayer> layer) {
	topLayers.push_back(move(layer));
}

void BackgroundLayer::addBottomLayer(unique_ptr<ClothLayer> layer) {
	bottomLayers.push_back(move(layer));
}
