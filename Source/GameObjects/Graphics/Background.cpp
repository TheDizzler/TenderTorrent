#include "../../pch.h"
#include "Background.h"
#include "../../../DXTKGui/StringHelper.h"
#include "../../Engine/GameEngine.h"

Background::Background() {


}

Background::~Background() {

	clear();

}

void Background::reloadGraphicsAssets() {
	baseBG->reloadGraphicsAsset(&guiFactory);
	for (const auto& layer : bgLayers)
		layer->reloadGraphicsAssets();
}

void Background::clear() {

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

	introScrollDone = false;
	waitDone = false;
	totalWaypointTime = 0;
}


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
	if (!bgLayerAsset->load(device, filepath.c_str(),
		StringHelper::convertCharStarToWCharT(filepath.c_str()))) {
		wstringstream wss;
		wss << "Unable to background image " << xmlFile;
		GameEngine::showErrorDialog(wss.str(), L"Fatal Error");
		return false;
	}
	baseBG = make_unique<Sprite>();
	baseBG->load(bgLayerAsset.get());
	baseBG->setOrigin(Vector2::Zero);
	bgLayerAssets.push_back(move(bgLayerAsset));


	float scaleFactor = levelRoot.attribute("scale").as_float();
	scaleFactor = 1;
	baseBG->setScale(Vector2(scaleFactor, scaleFactor));
	//camera.setZoom(scaleFactor);

	xml_node waypointsNode = levelRoot.child("waypoints");
	xml_node introWP = waypointsNode.child("intro");

	// set intro scroll
	Vector2 currentWPVector = Vector2(
		introWP.attribute("x").as_float(), introWP.attribute("y").as_float()) * scaleFactor;
	constrainToBackground(currentWPVector);
	lastWaypoint = new Waypoint(currentWPVector, 0);

	// set initial position of level
	xml_node startWP = waypointsNode.child("start");
	if (startWP.attribute("center").as_bool() == true) {
		currentWPVector = Vector2(float(baseBG->getWidth()) / 2, (float) baseBG->getHeight()) * scaleFactor;
		constrainToBackground(currentWPVector);
		currentWaypoint = new Waypoint(
			currentWPVector, startWP.attribute("speed").as_float());
	} else {
		currentWPVector = Vector2(
			startWP.attribute("x").as_float(), startWP.attribute("y").as_float()) * scaleFactor;
		constrainToBackground(currentWPVector);
		currentWaypoint = new Waypoint(currentWPVector, startWP.attribute("speed").as_float());
	}

	startWaypoint = currentWPVector;

	for (xml_node waypoint : waypointsNode.children("waypoint")) {
		currentWPVector = Vector2(
			waypoint.attribute("x").as_float(), waypoint.attribute("y").as_float()) * scaleFactor;
		constrainToBackground(currentWPVector);
		waypoints.push(new Waypoint(currentWPVector, waypoint.attribute("speed").as_float()));

	}

	// last waypoint
	xml_node endWP = waypointsNode.child("end");
	currentWPVector = Vector2(
		endWP.attribute("x").as_float(), endWP.attribute("y").as_float()) * scaleFactor;
	constrainToBackground(currentWPVector);
	waypoints.push(new Waypoint(currentWPVector, endWP.attribute("speed").as_float()));


	cornerFrame.reset(new Sprite());
	cornerFrame->load(gfxAssets.getAsset("Corner Frame"));

	camera.setLevel(this);
	camera.centerOn(lastWaypoint->dest);
	update(0);
	return loadLevel(device, levelRoot);
}



int Background::getWidth() {
	return baseBG->getWidth();
}

int Background::getHeight() {
	return baseBG->getHeight();
}

const Vector2& Background::getStartPosition() {
	//return lastWaypoint->dest;
	return startWaypoint;
}

const float CONSTANT = 150;
bool Background::startUpdate(double deltaTime) {

	if (introScrollDone)
		return true;

	totalWaypointTime += currentWaypoint->scrollSpeed * deltaTime;
	if (!waitDone) {
		if (totalWaypointTime >= 2)
			waitDone = true;
		return false;
	}

	double t = totalWaypointTime / CONSTANT;

	Vector2 newpos = Vector2::Lerp(lastWaypoint->dest, currentWaypoint->dest, (float) t);
	camera.centerOn(newpos);
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


bool Background::update(double deltaTime) {

	totalWaypointTime += currentWaypoint->scrollSpeed * deltaTime;
	double t = totalWaypointTime / CONSTANT;

	Vector2 newpos = Vector2::Lerp(lastWaypoint->dest, currentWaypoint->dest, (float) t);
	camera.centerOn(newpos);

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

	for (const auto& layer : bgLayers)
		layer->update(deltaTime);
	return false;
}

void Background::draw(SpriteBatch * batch) {

	baseBG->draw(batch);
	for (const auto& layer : bgLayers)
		layer->draw(batch);
}

vector<ClothLayer*> Background::getClothes() {

	vector<ClothLayer*> bgVec;
	for (const auto& bg : bgLayers) {
		bg->getLayers(bgVec);

	}
	return bgVec;
}



bool Background::loadLevel(ComPtr<ID3D11Device> device, xml_node levelRoot) {

	string dir = levelRoot.attribute("dir").as_string();
	string file_s = dir + levelRoot.child("backgroundSpriteSheet").text().as_string();

	const char_t* file = file_s.c_str();
	const char_t* masterAssetName = levelRoot.attribute("name").as_string();
	// the spritesheet itself is never saved
	unique_ptr<GraphicsAsset> masterAsset = make_unique<GraphicsAsset>();
	if (!masterAsset->load(device, masterAssetName,
		StringHelper::convertCharStarToWCharT(file), Vector2::Zero)) {
		GameEngine::errorMessage(L"Failed to load level asset file.");
		return false;
	}


	for (xml_node compoundLayerNode = levelRoot.child("backgroundLayer"); compoundLayerNode;
		compoundLayerNode = compoundLayerNode.next_sibling("backgroundLayer")) {

		unique_ptr<BackgroundLayer> compoundLayer = make_unique<BackgroundLayer>();

		const char_t* setName = compoundLayerNode.attribute("set").as_string();

		// parse top layers
		for (xml_node clothNode = compoundLayerNode.child("cloth"); clothNode;
			clothNode = clothNode.next_sibling("cloth")) {

			xml_node posNode = clothNode.child("position");
			// pos relative to level
			Vector2 worldPosition = Vector2(
				posNode.attribute("x").as_float(), posNode.attribute("y").as_float());
			// pos in sprite sheet
			Vector2 position = Vector2(
				clothNode.attribute("x").as_float(), clothNode.attribute("y").as_float());
			xml_node sizeNode = clothNode.child("size");
			Vector2 size = Vector2(
				sizeNode.attribute("x").as_float(), sizeNode.attribute("y").as_float());

			unique_ptr<GraphicsAsset> spriteAsset = make_unique<GraphicsAsset>();
			spriteAsset->loadAsPartOfSheet(masterAsset->getTexture(), setName,
				position, size, Vector2::Zero);


			unique_ptr<ClothLayer> bgLayer = make_unique<ClothLayer>();
			bgLayer->load(spriteAsset.get(), cornerFrame.get());
			//bgLayer->setLayerDepth(bgLayerNode.attribute("layer").as_float());
			bgLayer->setHealth(clothNode.attribute("health").as_int());
			bgLayerAssets.push_back(move(spriteAsset));
			//bgLayer->setMatrixFunction([&]() -> Matrix {return camera.translationMatrix(); });
			//bgLayer->setCameraZoom([&]() -> float { return camera.getZoom(); });
			bgLayer->setInitialPosition(worldPosition, baseBG->getScale());


			// parse all single sprites from spritesheet
			for (xml_node pieceNode = clothNode.child("tatter"); pieceNode;
				pieceNode = pieceNode.next_sibling("tatter")) {

				position = Vector2(
					pieceNode.attribute("x").as_float(), pieceNode.attribute("y").as_float());

				unique_ptr<GraphicsAsset> pieceAsset = make_unique<GraphicsAsset>();
				pieceAsset->loadAsPartOfSheet(masterAsset->getTexture(), setName,
					position, size, Vector2::Zero);

				bgLayer->loadPiece(pieceAsset.get());
				bgLayerAssets.push_back(move(pieceAsset));
			}

			compoundLayer->addTopLayer(move(bgLayer));

		}



		// parse bottom layers
		for (xml_node subLayerNode = compoundLayerNode.child("subLayer"); subLayerNode;
			subLayerNode = subLayerNode.next_sibling("subLayer")) {

			for (xml_node subClothNode = subLayerNode.child("cloth");
				subClothNode; subClothNode = subClothNode.next_sibling("cloth")) {

				xml_node posNode = subClothNode.child("position");
				// pos relative to level
				Vector2 worldPosition = Vector2(
					posNode.attribute("x").as_float(), posNode.attribute("y").as_float());
					// pos in sprite sheet
				Vector2 position = Vector2(
					subClothNode.attribute("x").as_float(), subClothNode.attribute("y").as_float());
				xml_node sizeNode = subClothNode.child("size");
				Vector2 size = Vector2(
					sizeNode.attribute("x").as_float(), sizeNode.attribute("y").as_float());

				unique_ptr<GraphicsAsset> spriteAsset = make_unique<GraphicsAsset>();
				spriteAsset->loadAsPartOfSheet(masterAsset->getTexture(), setName,
					position, size, Vector2::Zero);


				unique_ptr<ClothLayer> bgLayer = make_unique<ClothLayer>();
				bgLayer->load(spriteAsset.get(), cornerFrame.get());
				//bgLayer->setLayerDepth(bgLayerNode.attribute("layer").as_float());
				bgLayer->setHealth(subClothNode.attribute("health").as_int());
				bgLayerAssets.push_back(move(spriteAsset));
				//bgLayer->setMatrixFunction([&]() -> Matrix {return camera.translationMatrix(); });
				//bgLayer->setCameraZoom([&]() -> float { return camera.getZoom(); });
				bgLayer->setInitialPosition(worldPosition, baseBG->getScale());


				// parse all single sprites from spritesheet
				for (xml_node pieceNode = subClothNode.child("tatter"); pieceNode;
					pieceNode = pieceNode.next_sibling("tatter")) {

					position = Vector2(
						pieceNode.attribute("x").as_float(), pieceNode.attribute("y").as_float());

					xml_node sizeNode = pieceNode.child("size");
					if (sizeNode)
						size = Vector2(
							sizeNode.attribute("x").as_float(), sizeNode.attribute("y").as_float());
					unique_ptr<GraphicsAsset> pieceAsset = make_unique<GraphicsAsset>();
					pieceAsset->loadAsPartOfSheet(masterAsset->getTexture(), setName,
						position, size, Vector2::Zero);

					bgLayer->loadPiece(pieceAsset.get());
					bgLayerAssets.push_back(move(pieceAsset));
				}

				compoundLayer->addBottomLayer(move(bgLayer));
			}
		}

		bgLayers.push_back(move(compoundLayer));
	}

	return true;
}

void Background::constrainToBackground(Vector2& waypoint) {

	/*if (waypoint.x < camera->viewportWidth / 2)
		waypoint.x = camera->viewportWidth / 2;
	if (waypoint.x > getWidth() - camera->viewportWidth / 2)
		waypoint.x = getWidth() - camera->viewportWidth / 2;
	if (waypoint.y < camera->viewportHeight / 2)
		waypoint.y = camera->viewportHeight / 2;
	if (waypoint.y > getHeight() - camera->viewportHeight / 2)
		waypoint.y = getHeight() - camera->viewportHeight / 2;*/
}

