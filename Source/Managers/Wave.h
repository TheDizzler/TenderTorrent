#pragma once

#include "GFXAssetManager.h"
#include "../GameObjects/EnemyShip.h"


class Wave {
public:
	
	virtual ~Wave();

	virtual bool initialize(GFXAssetManager* gfxAssets, xml_node shipNode) = 0;

	void reloadGraphicsAssets();

	void clear();
	virtual void launchNewWave() = 0;

	void clearEnemies();

	/** Updates ships only (not bullets). Removes dead enemy bullets. */
	virtual void update(double deltaTime, PlayerShip* player);
	/** Draws both ships and enemy bullets. */
	void draw(SpriteBatch* batch);

	/* Update to play when level finished. */
	virtual void finishedUpdate(double deltaTime);

	vector<Bullet*> liveBullets;
	vector<EnemyShip*> shipStore;
	
protected:
	size_t MAX_SHIPS_IN_STORE = 2;
	size_t nextShipInStore = 0;
	
	virtual bool checkForLaunch();

	double timeBetweenChecks = 3;
	double timeUntilNextCheck = timeBetweenChecks;
	double timeSinceLastLaunch = 0;
	unsigned int maxTimeBetweenLaunches = 25;


private:


};