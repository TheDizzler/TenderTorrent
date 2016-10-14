#pragma once

#include <pugixml.hpp>
#include "BackgroundLayer.h"

using namespace pugi;


class Background : public Sprite {
public:
	Background();
	~Background();

	virtual bool load(ComPtr<ID3D11Device> device, const char_t* levelFile);
	void clear();

	void update(double deltaTime, PlayerShip* player);
	virtual void draw(SpriteBatch* batch) override;


	vector<BackgroundLayer*> bgLayers;

private:

	Vector2 startPos;
	unique_ptr<Sprite> cornerFrame;
	vector<unique_ptr<GraphicsAsset> > bgLayerAssets;

	bool loadLevel(ComPtr<ID3D11Device> device, xml_node levelRoot);
};