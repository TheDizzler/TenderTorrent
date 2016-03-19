#include "Background.h"

Background::Background() : Sprite() {


}

Background::~Background() {
}

bool Background::load(ID3D11Device * device, const wchar_t * file) {

	if (!Sprite::load(device, file))
		return false;


	position.x += width / 2;

	/*for (int i = 0; i < Assets::numMako01Files; ++i) {
		wchar_t* filename = Assets::bgMako01File + L"a";
	}*/

	BackgroundLayer* bgLayer = new BackgroundLayer();
	if (!bgLayer->load(device, L"assets/bg/mako01a.dds"))
		return false;
	bgLayer->setPosition(position);
	bgLayer->setHitArea(Assets::aPos, Assets::aSize);
	bgLayers.push_back(bgLayer);

	bgLayer = new BackgroundLayer();
	if (!bgLayer->load(device, L"assets/bg/mako01b.dds"))
		return false;
	bgLayer->setPosition(position);
	bgLayer->setHitArea(Assets::bPos, Assets::bSize);
	bgLayers.push_back(bgLayer);

	bgLayer = new BackgroundLayer();
	if (!bgLayer->load(device, L"assets/bg/mako01c.dds"))
		return false;
	bgLayer->setPosition(position);
	bgLayer->setHitArea(Assets::cPos, Assets::cSize);
	bgLayers.push_back(bgLayer);

	bgLayer = new BackgroundLayer();
	if (!bgLayer->load(device, L"assets/bg/mako01d.dds"))
		return false;
	bgLayer->setPosition(position);
	bgLayer->setHitArea(Assets::dPos, Assets::dSize);

	bgLayers.push_back(bgLayer);

	bgLayer = new BackgroundLayer();
	if (!bgLayer->load(device, L"assets/bg/mako01e.dds"))
		return false;
	bgLayer->setPosition(position);
	bgLayer->setHitArea(Assets::ePos, Assets::eSize);
	bgLayers.push_back(bgLayer);

	bgLayer = new BackgroundLayer();
	if (!bgLayer->load(device, L"assets/bg/mako01f.dds"))
		return false;
	bgLayer->setPosition(position);
	bgLayer->setHitArea(Assets::fPos, Assets::fSize);
	bgLayers.push_back(bgLayer);

	/*bgLayer = new BackgroundLayer();
	if (!bgLayer->load(device, L"assets/bg/mako01g.dds"))
		return false;
	bgLayer->setPosition(position);
	bgLayers.push_back(bgLayer);*/

	/*bgLayer = new BackgroundLayer();
	if (!bgLayer->load(device, L"assets/bg/mako01h.dds"))
		return false;
	bgLayer->setPosition(position);
	bgLayers.push_back(bgLayer);*/

	/*bgLayer = new BackgroundLayer();
	if (!bgLayer->load(device, L"assets/bg/mako01i.dds"))
		return false;
	bgLayer->setPosition(position);
	bgLayers.push_back(bgLayer);*/

	return true;
}


void Background::update(double deltaTime, PlayerShip* player) {

	Vector2 posChange = Vector2(0, 10 * deltaTime);
	position.y += posChange.y;

	for (BackgroundLayer* layer : bgLayers)
		layer->setPosition(posChange);
}

void Background::draw(SpriteBatch * batch) {

	Sprite::draw(batch);
	for (BackgroundLayer* layer : bgLayers)
		layer->draw(batch);
}


