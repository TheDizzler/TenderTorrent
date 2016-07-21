#pragma once

#include <vector>

#include <pugixml.hpp>
#include "BackgroundLayer.h"

using namespace pugi;


class Background : public Sprite {
public:
	Background();
	~Background();

	virtual bool load(ID3D11Device* device, const wchar_t* file) override;
	void clear();

	void update(double deltaTime, PlayerShip* player);
	virtual void draw(SpriteBatch* batch) override;


	std::vector<BackgroundLayer*> bgLayers;

private:

	Vector2 startPos;
	unique_ptr<FontSet> healthFont;
	unique_ptr<Sprite> cornerFrame;

	bool loadLevel(ID3D11Device* device, xml_node levelRoot);
};