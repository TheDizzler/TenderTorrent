#include "../pch.h"
#pragma once

#include "GFXAssetManager.h"
#include "../GameObjects/EnemyShip.h"


class Wave {
public:
	Wave();
	~Wave();

	virtual bool initialize(GFXAssetManager* gfxAssets) = 0;
	void clear();
	virtual void launchNewWave() = 0;

	/** Updates ships only. Removes dead enemy bullets. */
	virtual void update(double deltaTime, PlayerShip* player);
	/** Draws both ships and enemy bullets. */
	void draw(SpriteBatch* batch);


	vector<EnemyShip*> enemyShips;
	vector<Bullet*> bullets;
protected:

	virtual bool checkForLaunch() = 0;

	unique_ptr<Sprite> sharedShipSprite;
	unique_ptr<Sprite> sharedBulletSprite;

	
	const double timeBetweenChecks = 3;
	double timeUntilNextCheck = timeBetweenChecks;
	double timeSinceLastLaunch = 0;
	int maxTimeBetweenLaunches = 25;
private:


};