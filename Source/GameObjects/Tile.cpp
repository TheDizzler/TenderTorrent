#include "Tile.h"

Tile::Tile() :Sprite(SimpleMath::Vector2(0, 0)) {
}

Tile::Tile(const Vector2 & position, bool isPssbl) : Sprite(position) {


	hitArea = new HitArea(position, Vector2(width, height));
	isPassable = isPssbl;
}


Tile::~Tile() {
}

void Tile::draw(SpriteBatch* batch, Tile* baseTile) {

	batch->Draw(baseTile->texture, position, &(baseTile->sourceRect), tint, rotation, baseTile->origin, scale, SpriteEffects_None, layerDepth);

}

ID3D11ShaderResourceView* Tile::shareTexture() {
	return texture;
}

