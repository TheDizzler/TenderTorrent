#include "CompoundLayer.h"

CompoundLayer::CompoundLayer() {


}

CompoundLayer::~CompoundLayer() {
}

void CompoundLayer::addTopLayer(unique_ptr<ClothLayer> layer) {
	topLayers.push_back(move(layer));
}

void CompoundLayer::addBottomLayer(unique_ptr<ClothLayer> layer) {
	bottomLayers.push_back(move(layer));
}
