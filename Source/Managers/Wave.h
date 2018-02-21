#pragma once

#include "GFXAssetManager.h"
#include "../GameObjects/Enemies/EnemyShip.h"


class Wave {
public:
	
	virtual ~Wave();

	virtual bool initialize(GFXAssetManager* gfxAssets, xml_node shipNode) = 0;

	void reloadGraphicsAssets();

	void clear();
	

	void clearEnemies();

	void checkHitDetection(Bullet* bullet);
	/** Updates ships only (not bullets). Removes dead enemy bullets. */
	virtual void update(double deltaTime, PlayerShip* player);
	/** Draws both ships and enemy bullets. */
	void draw(SpriteBatch* batch);

	/* Update to play when level finished. */
	virtual void finishedUpdate(double deltaTime);

	vector<Bullet*> liveBullets;
	vector<EnemyShip*> shipStore;
	size_t shipsLaunched = 0;
protected:
	size_t MAX_SHIPS_IN_STORE = 2;
	size_t nextShipInStore = 0;
	
	virtual void launchNewWave() = 0;
	
	bool checkForLaunch();

	double timeBetweenChecks = 3;
	double timeUntilNextCheck = timeBetweenChecks;
	double timeSinceLastLaunch = 0;
	unsigned int maxTimeBetweenLaunches = 25;


private:


};