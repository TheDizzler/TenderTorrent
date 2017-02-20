#include "../../pch.h"
#pragma once

#include "BackgroundLayer.h"

class CompoundLayer : public BackgroundLayer {
public:
	CompoundLayer();
	~CompoundLayer();

	void addTopLayer(unique_ptr<ClothLayer> layer);
	void addBottomLayer(unique_ptr<ClothLayer> layer);
private:

	vector<unique_ptr<ClothLayer> > topLayers;
	vector<unique_ptr<ClothLayer> > bottomLayers;

};