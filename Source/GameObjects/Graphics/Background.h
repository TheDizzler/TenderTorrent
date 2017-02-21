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
	bool update(double deltaTime, shared_ptr<MouseController> mouse);
	void draw(SpriteBatch* batch);

	vector</*unique_ptr<ClothLayer>*/ClothLayer*> getClothes();
	

	bool introScrollDone = false;
private:

	queue<Waypoint*> waypoints;
	Waypoint* currentWaypoint = NULL;
	Waypoint* lastWaypoint = NULL;

	unique_ptr<Sprite> baseBG;
	shared_ptr<Sprite> cornerFrame;
	vector<unique_ptr<GraphicsAsset> > bgLayerAssets;
	vector<unique_ptr<BackgroundLayer>> bgLayers;

	bool loadLevel(ComPtr<ID3D11Device> device, xml_node levelRoot);

	void constrainToBackground(Vector2& waypoint);

	float scrollSpeed = 15;

	
	bool waitDone = false;
	double totalWaypointTime = 0;
};