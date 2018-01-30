#include "../../pch.h"
#include "BackgroundLayer.h"

BackgroundLayer::BackgroundLayer() {
}

BackgroundLayer::~BackgroundLayer() {

	/*for (ClothLayer* layer : topLayers)
		delete layer;*/
	topLayers.clear();

	/*for (ClothLayer* layer : bottomLayers)
		delete layer;*/
	bottomLayers.clear();
}

void BackgroundLayer::reloadGraphicsAssets() {
	for (const auto& layer : bottomLayers)
		layer->reloadGraphicsAssets();
	for (const auto& layer : topLayers)
		layer->reloadGraphicsAssets();
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

void BackgroundLayer::getLayers(vector<ClothLayer*>& bgLayers) {

	if (topDone)
		//bgLayers.insert(bgLayers.end(), bottomLayers.begin(), bottomLayers.end());
		for (const auto& layer : bottomLayers)
			bgLayers.push_back(layer.get());

	else
		//bgLayers.insert(bgLayers.end(), topLayers.begin(), topLayers.end());
		for (const auto& layer : topLayers)
			bgLayers.push_back(layer.get());
}


void BackgroundLayer::addTopLayer(unique_ptr<ClothLayer> layer) {
	topLayers.push_back(move(layer));
}

void BackgroundLayer::addBottomLayer(unique_ptr<ClothLayer> layer) {
	bottomLayers.push_back(move(layer));
}
