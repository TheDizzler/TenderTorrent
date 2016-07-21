#include "Background.h"

Background::Background() : Sprite() {


}

Background::~Background() {

	for each (BackgroundLayer* bgLayer in bgLayers)
		delete bgLayer;
}

bool Background::load(ID3D11Device* device, const wchar_t* file) {

	xml_document doc;

	if (!doc.load_file(file)) {
		MessageBox(NULL, L"Failed to load xml file", L"ERROR", MB_OK);
		return false;
	}

	xml_node levelRoot = doc.first_child();

	const char_t* baseFile = levelRoot.child("base").text().as_string();

	if (!Sprite::load(device, Assets::convertCharStarToWCharT(baseFile)))
		return false;

	float scaleFactor = levelRoot.attribute("scale").as_float();
	scale = Vector2(scaleFactor, scaleFactor);


	position.x += width*scale.x / 2; // set initial position of level
	position.y -= height*scale.y / 2;


	healthFont.reset(new FontSet());
	if (!healthFont->load(device, Assets::arialFontFile))
		return false;
	healthFont->setTint(DirectX::Colors::Black.v);

	cornerFrame.reset(new Sprite());
	if (!cornerFrame->load(device, Assets::cornerFrameFile))
		return false;

	return loadLevel(device, levelRoot);
}

void Background::clear() {

	for each (BackgroundLayer* bgLayer in bgLayers)
		delete bgLayer;

	bgLayers.clear();
}


bool Background::loadLevel(ID3D11Device * device, xml_node levelRoot) {


	for each (xml_node layerNode in levelRoot.children("backgroundLayer")) {

		BackgroundLayer* bgLayer = new BackgroundLayer();
		string file = layerNode.text().as_string();

		Assets::trim(file);
		if (!bgLayer->load(device, Assets::convertCharStarToWCharT(file.c_str())))
			return false;
		bgLayer->setInitialPosition(position, scale);

		Vector2 pos = Vector2(layerNode.child("position").attribute("x").as_int(),
			layerNode.child("position").attribute("y").as_int());

		Vector2 size = Vector2(layerNode.child("size").attribute("x").as_int(),
			layerNode.child("size").attribute("y").as_int());


		bgLayer->setHitArea(pos, size, healthFont.get());
		bgLayers.push_back(bgLayer);

	}

	return true;
}


void Background::update(double deltaTime, PlayerShip* player) {

	Vector2 posChange = Vector2(0, 15 * deltaTime);
	position += posChange;

	for (BackgroundLayer* layer : bgLayers)
		layer->setPosition(posChange);
}

void Background::draw(SpriteBatch * batch) {

	Sprite::draw(batch);
	for (BackgroundLayer* layer : bgLayers)
		layer->draw(batch, cornerFrame.get());
}


