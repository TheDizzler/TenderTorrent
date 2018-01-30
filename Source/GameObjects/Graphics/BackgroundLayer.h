#pragma once

#include "ClothLayer.h"


class BackgroundLayer {
public:
	BackgroundLayer();
	virtual ~BackgroundLayer();

	void reloadGraphicsAssets();

	void update(double deltaTime);
	void draw(SpriteBatch* batch);

	void getLayers(vector<ClothLayer*>& bgLayers);
	//vector<ClothLayer*> getLayers();

	void addTopLayer(unique_ptr<ClothLayer> layer);
	void addBottomLayer(unique_ptr<ClothLayer> layer);
	/*void addTopLayer(ClothLayer* layer);
	void addBottomLayer(ClothLayer* layer);*/
private:

	vector<unique_ptr<ClothLayer> > topLayers;
	vector<unique_ptr<ClothLayer> > bottomLayers;
	/*vector<ClothLayer*> topLayers;
	vector<ClothLayer*> bottomLayers;*/
	bool topDone = true;

};