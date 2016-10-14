#include "Background.h"

Background::Background() : Sprite() {


}

Background::~Background() {

	for each (BackgroundLayer* bgLayer in bgLayers)
		delete bgLayer;

	bgLayerAssets.clear();
}

#include "../../DXTKGui/StringHelper.h"
#include "../../Engine/GameEngine.h"
bool Background::load(ComPtr<ID3D11Device> device, const char_t* xmlFile) {

	xml_document doc;

	if (!doc.load_file(xmlFile)) {
		MessageBox(NULL, L"Failed to load xml file", L"ERROR", MB_OK);
		return false;
	}

	xml_node levelRoot = doc.first_child();

	string filepath = levelRoot.attribute("dir").as_string();
	filepath += levelRoot.child("base").text().as_string();
	//const char_t* baseFile = 


	unique_ptr<GraphicsAsset> bgLayerAsset;
	bgLayerAsset.reset(new GraphicsAsset());
	if (!bgLayerAsset->load(device, StringHelper::convertCharStarToWCharT(filepath.c_str()))) {
		wstringstream wss;
		wss << "Unable to background image " << xmlFile;
		GameEngine::showErrorDialog(wss.str(), L"Fatal Error");
		return false;
	}

	Sprite::load(bgLayerAsset.get());
	bgLayerAssets.push_back(move(bgLayerAsset));


	float scaleFactor = levelRoot.attribute("scale").as_float();
	scale = Vector2(scaleFactor, scaleFactor);


	position.x += width*scale.x / 2; // set initial position of level
	position.y -= height*scale.y / 2;


	/*healthFont = move(GameManager::guiFactory->getFont("Arial"));
	healthFont->setTint(Vector4(1, 1, 1, 1));*/


	cornerFrame.reset(new Sprite());
	cornerFrame->load(GameManager::gfxAssets->getAsset("Corner Frame"));

	return loadLevel(device, levelRoot);
}

void Background::clear() {

	for each (BackgroundLayer* bgLayer in bgLayers)
		delete bgLayer;

	bgLayers.clear();
	bgLayerAssets.clear();
}


bool Background::loadLevel(ComPtr<ID3D11Device> device, xml_node levelRoot) {

	string dir = levelRoot.attribute("dir").as_string();
	for each (xml_node layerNode in levelRoot.children("backgroundLayer")) {

		BackgroundLayer* bgLayer = new BackgroundLayer();
		string file = layerNode.text().as_string();
		StringHelper::trim(file);
		string filepath = dir + file;

		
		unique_ptr<GraphicsAsset> layerAsset;
		layerAsset.reset(new GraphicsAsset());
		if (!layerAsset->load(device, StringHelper::convertCharStarToWCharT(filepath.c_str()))) {
			wstringstream wss;
			wss << "Unable to background image " << filepath.c_str();
			GameEngine::showErrorDialog(wss.str(), L"Fatal Error");
			return false;
		}


		bgLayer->load(layerAsset.get());
		bgLayer->setInitialPosition(position, scale);

		bgLayerAssets.push_back(move(layerAsset));

		Vector2 pos = Vector2(layerNode.child("position").attribute("x").as_int(),
			layerNode.child("position").attribute("y").as_int());

		Vector2 size = Vector2(layerNode.child("size").attribute("x").as_int(),
			layerNode.child("size").attribute("y").as_int());


		bgLayer->setHitArea(pos, size);
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


