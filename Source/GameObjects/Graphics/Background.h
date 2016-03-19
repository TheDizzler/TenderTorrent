#pragma once

#include <vector>
#include "BackgroundLayer.h"


class Background : public Sprite {
public:
	Background();
	~Background();

	virtual bool load(ID3D11Device* device, const wchar_t* file) override;

	void update(double deltaTime, PlayerShip* player);
	virtual void draw(SpriteBatch* batch) override;


	std::vector<BackgroundLayer*> bgLayers;

private:

	

	Vector2 startPos;
};