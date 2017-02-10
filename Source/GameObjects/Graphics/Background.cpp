#include "Background.h"

Background::Background() {


}

Background::~Background() {

	clear();

}

void Background::clear() {

	/*for each (BackgroundLayer* bgLayer in bgLayers)
		delete bgLayer;*/
	bgLayers.clear();

	bgLayerAssets.clear();

	while (waypoints.size() > 0) {
		delete waypoints.front();
		waypoints.pop();
	}

	if (currentWaypoint)
		delete currentWaypoint;
	currentWaypoint = NULL;
	if (lastWaypoint)
		delete lastWaypoint;
	lastWaypoint = NULL;
}

#include "../../DXTKGui/StringHelper.h"
#include "../../Engine/GameEngine.h"
bool Background::loadLevel(ComPtr<ID3D11Device> device, const char_t* xmlFile) {

	xml_document doc;

	if (!doc.load_file(xmlFile)) {
		//MessageBox(NULL, L"Failed to load xml file", L"ERROR", MB_OK);
		return false;
	}

	xml_node levelRoot = doc.first_child();

	string filepath = levelRoot.attribute("dir").as_string();
	filepath += levelRoot.child("base").text().as_string();


	unique_ptr<GraphicsAsset> bgLayerAsset;
	bgLayerAsset.reset(new GraphicsAsset());
	if (!bgLayerAsset->load(device, StringHelper::convertCharStarToWCharT(filepath.c_str()))) {
		wstringstream wss;
		wss << "Unable to background image " << xmlFile;
		GameEngine::showErrorDialog(wss.str(), L"Fatal Error");
		return false;
	}
	baseBG = make_unique<Sprite>();
	baseBG->load(bgLayerAsset.get());
	baseBG->setOrigin(Vector2::Zero);
	bgLayerAssets.push_back(move(bgLayerAsset));


	float scaleFactor = levelRoot.attribute("scale").as_float(); // change this camera zoom?
	//baseBG->setScale(Vector2(scaleFactor, scaleFactor));
	camera->setZoom(scaleFactor);

	xml_node waypointsNode = levelRoot.child("waypoints");
	xml_node introWP = waypointsNode.child("intro");

	// set intro scroll
	lastWaypoint = new Waypoint(
		Vector2(introWP.attribute("x").as_int(), introWP.attribute("y").as_int()), 0);

	// set initial position of level
	xml_node startWP = waypointsNode.child("start");
	if (startWP.attribute("center").as_bool() == true) {
		//position.x += width*scale.x / 2; 
		//position.y -= height*scale.y / 2;
		currentWaypoint = new Waypoint(
			Vector2(baseBG->getWidth() / 2, baseBG->getHeight()), startWP.attribute("speed").as_float());
	} else {
		currentWaypoint = new Waypoint(
			Vector2(startWP.attribute("x").as_int(), startWP.attribute("y").as_int()),
			startWP.attribute("speed").as_float());
	}

	for (xml_node waypoint : waypointsNode.children("waypoint")) {
		waypoints.push(new Waypoint(
			Vector2(waypoint.attribute("x").as_int(), waypoint.attribute("y").as_int()),
			waypoint.attribute("speed").as_float()));

	}

	// last waypoint
	xml_node endWP = waypointsNode.child("end");
	waypoints.push(new Waypoint(
		Vector2(endWP.attribute("x").as_int(), endWP.attribute("y").as_int()),
		endWP.attribute("speed").as_float()));


	cornerFrame.reset(new Sprite());
	cornerFrame->load(gfxAssets->getAsset("Corner Frame"));

	camera->setLevel(this);
	camera->centerOn(lastWaypoint->dest);
	//currentWaypoint = waypoints.front();
	//waypoints.pop();

	return loadLevel(device, levelRoot);
}



int Background::getWidth() {
	return baseBG->getWidth();
}

int Background::getHeight() {
	return baseBG->getHeight();
}

const Vector2& Background::getStartPosition() {
	return lastWaypoint->dest;
}

bool introScrollDone = false;

float CONSTANT = 150;
double totalWaypointTime = 0;
bool waitDone = false;
bool Background::startUpdate(double deltaTime) {

	if (introScrollDone)
		return introScrollDone;

	totalWaypointTime += currentWaypoint->scrollSpeed * deltaTime;
	if (!waitDone) {
		if (totalWaypointTime >= 2)
			waitDone = true;
		return false;
	}

	double t = totalWaypointTime / CONSTANT;

	Vector2 newpos = Vector2::Lerp(lastWaypoint->dest, currentWaypoint->dest, t);
	camera->setCameraPosition(newpos);
	if (t >= 1) {
		totalWaypointTime = 0;
		delete lastWaypoint;
		lastWaypoint = currentWaypoint;
		currentWaypoint = waypoints.front();
		waypoints.pop();
		introScrollDone = true;
		return true;
	}
	return false;
}


bool Background::update(double deltaTime, PlayerShip* player) {

	totalWaypointTime += currentWaypoint->scrollSpeed * deltaTime;
	double t = totalWaypointTime / CONSTANT;

	Vector2 newpos = Vector2::Lerp(lastWaypoint->dest, currentWaypoint->dest, t);
	camera->setCameraPosition(newpos);

	if (t >= 1) {
		totalWaypointTime = 0;
		delete lastWaypoint;
		lastWaypoint = currentWaypoint;
		currentWaypoint = NULL;
		if (waypoints.size() == 0) {
			return true;
		} else {
			currentWaypoint = waypoints.front();
			waypoints.pop();
		}
	}
	//for (BackgroundLayer* layer : bgLayers)
	for (const auto& layer : bgLayers)
		layer->updateProjectedHitArea();
	return false;
}

void Background::draw(SpriteBatch * batch) {

	baseBG->draw(batch);
	//for (BackgroundLayer* layer : bgLayers)
	for (const auto& layer : bgLayers)
		layer->draw(batch, cornerFrame.get());
}



bool Background::loadLevel(ComPtr<ID3D11Device> device, xml_node levelRoot) {

	string dir = levelRoot.attribute("dir").as_string();
	string file_s = dir + levelRoot.child("backgroundSpriteSheet").text().as_string();

	const char_t* file = file_s.c_str();

	// the spritesheet itself is never saved
	unique_ptr<GraphicsAsset> masterAsset;
	masterAsset.reset(new GraphicsAsset());
	if (!masterAsset->load(device, StringHelper::convertCharStarToWCharT(file))) {
		MessageBox(0, L"Failed", L"Failed", MB_OK);
		return false;
	}

	for (xml_node bgLayerNode = levelRoot.child("backgroundLayer");
		bgLayerNode; bgLayerNode = bgLayerNode.next_sibling("backgroundLayer")) {

		xml_node clothNode = bgLayerNode.child("cloth");
		xml_node posNode = clothNode.child("position");
		// pos relative to level
		Vector2 worldPosition = Vector2(posNode.attribute("x").as_int(), posNode.attribute("y").as_int());
		// pos in sprite sheet
		Vector2 position = Vector2(clothNode.attribute("x").as_int(), clothNode.attribute("y").as_int());
		xml_node sizeNode = clothNode.child("size");
		Vector2 size = Vector2(sizeNode.attribute("x").as_int(), sizeNode.attribute("y").as_int());

		unique_ptr<GraphicsAsset> spriteAsset = make_unique<GraphicsAsset>();
		spriteAsset->loadAsPartOfSheet(masterAsset->getTexture(), position, size, Vector2::Zero);


		unique_ptr<BackgroundLayer> bgLayer = make_unique<BackgroundLayer>();
		bgLayer->setLayerDepth(bgLayerNode.attribute("layer").as_float());
		bgLayer->load(spriteAsset.get());
		bgLayer->setHealth(bgLayerNode.attribute("health").as_int());
		bgLayerAssets.push_back(move(spriteAsset));
		bgLayer->setInitialPosition(worldPosition, baseBG->getScale());
		bgLayer->setMatrixFunction([&]() -> Matrix {return camera->translationMatrix(); });



		// parse all single sprites from spritesheet
		for (xml_node pieceNode = bgLayerNode.child("piece"); pieceNode;
			pieceNode = pieceNode.next_sibling("piece")) {

			position = Vector2(pieceNode.attribute("x").as_int(), pieceNode.attribute("y").as_int());

			unique_ptr<GraphicsAsset> pieceAsset = make_unique<GraphicsAsset>();
			pieceAsset->loadAsPartOfSheet(masterAsset->getTexture(), position, size, Vector2::Zero);

			bgLayer->loadPiece(pieceAsset.get());
			bgLayerAssets.push_back(move(pieceAsset));
		}
		bgLayers.push_back(move(bgLayer));
	}


	/*for each (xml_node layerNode in levelRoot.children("backgroundLayer")) {

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
		bgLayer->setOrigin(Vector2::Zero);
		bgLayer->setInitialPosition(baseBG->getPosition(), baseBG->getScale());
		bgLayer->setMatrixFunction([&]() -> Matrix {return camera->translationMatrix(); });
		bgLayerAssets.push_back(move(layerAsset));

		Vector2 pos = Vector2(layerNode.child("position").attribute("x").as_int(),
			layerNode.child("position").attribute("y").as_int());

		Vector2 size = Vector2(layerNode.child("size").attribute("x").as_int(),
			layerNode.child("size").attribute("y").as_int());


		bgLayer->setHitArea(pos, size);
		bgLayers.push_back(bgLayer);

	}*/

	return true;
}

