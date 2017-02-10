#include "../../pch.h"
#pragma once

#include <queue>
#include "BackgroundLayer.h"


struct Waypoint {
	Waypoint(Vector2 destination, float speed) : dest(destination), scrollSpeed(speed) {
	}
	~Waypoint() {}
	Vector2 dest;
	float scrollSpeed = 15;
};

class Background /*: public Sprite*/ {
public:
	Background();
	~Background();

	virtual bool loadLevel(ComPtr<ID3D11Device> device, const char_t* levelFile);
	void clear();

	int getWidth();
	int getHeight();

	const Vector2& getStartPosition();

	bool startUpdate(double deltaTime);
	bool update(double deltaTime, PlayerShip* player);
	void draw(SpriteBatch* batch);

	vector<unique_ptr<BackgroundLayer>> bgLayers;
	//vector<BackgroundLayer*> bgLayers;

	//bool levelFinished = false;
private:

	queue<Waypoint*> waypoints;
	Waypoint* currentWaypoint = NULL;
	Waypoint* lastWaypoint = NULL;

	unique_ptr<Sprite> baseBG;
	unique_ptr<Sprite> cornerFrame;
	vector<unique_ptr<GraphicsAsset> > bgLayerAssets;

	bool loadLevel(ComPtr<ID3D11Device> device, xml_node levelRoot);

	float scrollSpeed = 15;
};