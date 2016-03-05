#pragma once

#include "Graphics/Sprite.h"

class Tile : public Sprite {

public:
	/** For constructing base tiles */
	Tile();
	/** For constructing actual tiles seen on stage. */
	Tile(const Vector2& position, bool isPassable);
	virtual ~Tile();

	void draw(SpriteBatch* batch, Tile* baseTile);
	ID3D11ShaderResourceView* shareTexture();
	//void setSharedTexture(Tile* tileWithSharedTexture);

	//void releaseSharedTexture();

protected:
	bool isPassable;

};