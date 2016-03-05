#include "FontSet.h"


FontSet::FontSet() {

	//position = Vector2(200, 200);
	
	alpha = 1.0f;
	rotation = 0.0f;
	scale = Vector2(1, 1);
	tint = DirectX::Colors::White.v;
	layerDepth = 1.0f;
}

FontSet::~FontSet() {
}


bool FontSet::load(ID3D11Device* device, const wchar_t* file) {
	
	font.reset(new SpriteFont(device, file));
	return true;
}


void FontSet::draw(SpriteBatch* batch, const wchar_t* text, Vector2 pos) {
	
	font->DrawString(batch, text, pos, tint, rotation, origin, scale, SpriteEffects_None, layerDepth);
}


/** Don't use this draw function except for testing!! */
void FontSet::draw(SpriteBatch* batch) {

	font->DrawString(batch, defaultText, Vector2(0, 0));

}


const Vector2& FontSet::getPosition() const {

	return Vector2(0, 0);
}


const Vector2& FontSet::getOrigin() const {

	return origin;
}

const Vector2& FontSet::getScale() const {

	return scale;
}

const float FontSet::getRotation() const {
	return rotation;
}


const Color& FontSet::getTint() const {

	return tint;
}


const float FontSet::getAlpha() const {

	return alpha;
}


void FontSet::setPostion(const Vector2& position) {

}


void FontSet::setOrigin(const Vector2& origin) {


}


void FontSet::setScale(const Vector2& scale) {


}


void FontSet::setRotation(const float rotation) {

}


void FontSet::setTint(const Color& color) {

	tint = color;
}


void FontSet::setAlpha(const float alpha) {

}
