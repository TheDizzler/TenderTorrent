#include "../../pch.h"
#pragma once

#include "ClothLayer.h"


class BackgroundLayer {
public:
	BackgroundLayer();
	~BackgroundLayer();

	void update(double deltaTime);
	void draw(SpriteBatch* batch);

	void getLayers(vector<unique_ptr<ClothLayer>>& bgLayers);

	void addTopLayer(unique_ptr<ClothLayer> layer);
	void addBottomLayer(unique_ptr<ClothLayer> layer);
private:

	vector<unique_ptr<ClothLayer> > topLayers;
	vector<unique_ptr<ClothLayer> > bottomLayers;

	bool topDone = true;

};