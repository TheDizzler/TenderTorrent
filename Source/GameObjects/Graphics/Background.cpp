#include "Background.h"

Background::Background() : Sprite() {


}

Background::~Background() {
}

bool Background::load(ID3D11Device * device, const wchar_t * file) {

	if (!Sprite::load(device, file))
		return false;

	scale = Vector2(1.5, 1.5);


	position.x += width*scale.x / 2; // set initial position of level
	position.y -= height*scale.y / 4;

	/*for (int i = 0; i < Assets::numMako01Files; ++i) {
		wchar_t* filename = Assets::bgMako01File + L"a";
	}*/

	healthFont.reset(new FontSet());
	if (!healthFont->load(device, Assets::arialFontFile))
		return false;
	healthFont->setTint(DirectX::Colors::Black.v);

	BackgroundLayer* bgLayer = new BackgroundLayer();
	if (!bgLayer->load(device, L"assets/bg/mako01a.dds"))
		return false;
	bgLayer->setInitialPosition(position, scale);
	bgLayer->setHitArea(Assets::aPos, Assets::aSize, healthFont.get());
	bgLayers.push_back(bgLayer);

	bgLayer = new BackgroundLayer();
	if (!bgLayer->load(device, L"assets/bg/mako01b.dds"))
		return false;
	bgLayer->setInitialPosition(position, scale);
	bgLayer->setHitArea(Assets::bPos, Assets::bSize, healthFont.get());
	bgLayers.push_back(bgLayer);

	bgLayer = new BackgroundLayer();
	if (!bgLayer->load(device, L"assets/bg/mako01c.dds"))
		return false;
	bgLayer->setInitialPosition(position, scale);
	bgLayer->setHitArea(Assets::cPos, Assets::cSize, healthFont.get());
	bgLayers.push_back(bgLayer);

	bgLayer = new BackgroundLayer();
	if (!bgLayer->load(device, L"assets/bg/mako01d.dds"))
		return false;
	bgLayer->setInitialPosition(position, scale);
	bgLayer->setHitArea(Assets::dPos, Assets::dSize, healthFont.get());

	bgLayers.push_back(bgLayer);

	bgLayer = new BackgroundLayer();
	if (!bgLayer->load(device, L"assets/bg/mako01e.dds"))
		return false;
	bgLayer->setInitialPosition(position, scale);
	bgLayer->setHitArea(Assets::ePos, Assets::eSize, healthFont.get());
	bgLayers.push_back(bgLayer);

	bgLayer = new BackgroundLayer();
	if (!bgLayer->load(device, L"assets/bg/mako01f.dds"))
		return false;
	bgLayer->setInitialPosition(position, scale);
	bgLayer->setHitArea(Assets::fPos, Assets::fSize, healthFont.get());
	bgLayers.push_back(bgLayer);

	bgLayer = new BackgroundLayer();
	if (!bgLayer->load(device, L"assets/bg/mako01g.dds"))
		return false;
	bgLayer->setInitialPosition(position, scale);
	bgLayer->setHitArea(Assets::gPos, Assets::gSize, healthFont.get());
	bgLayers.push_back(bgLayer);

	bgLayer = new BackgroundLayer();
	if (!bgLayer->load(device, L"assets/bg/mako01h.dds"))
		return false;
	bgLayer->setInitialPosition(position, scale);
	bgLayer->setHitArea(Assets::hPos, Assets::hSize, healthFont.get());
	bgLayers.push_back(bgLayer);

	bgLayer = new BackgroundLayer();
	if (!bgLayer->load(device, L"assets/bg/mako01i.dds"))
		return false;
	bgLayer->setInitialPosition(position, scale);
	bgLayer->setHitArea(Assets::iPos, Assets::iSize, healthFont.get());
	bgLayers.push_back(bgLayer);

	return true;
}


void Background::update(double deltaTime, PlayerShip* player) {

	Vector2 posChange = Vector2(0, 10 * deltaTime);
	position += posChange;

	for (BackgroundLayer* layer : bgLayers)
		layer->setPosition(posChange);
}

void Background::draw(SpriteBatch * batch) {

	Sprite::draw(batch);
	for (BackgroundLayer* layer : bgLayers)
		layer->draw(batch);
}


