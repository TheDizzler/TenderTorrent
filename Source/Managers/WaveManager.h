#pragma once
#include "Wave.h"


class WaveManager {
public:
	virtual ~WaveManager();

	bool initialize(GFXAssetManager* gfxAssets);
	void reloadGraphicsAssets();

	void clear();
	void clearEnemies();

	int getBulletCount();
	void update(double deltaTime, PlayerShip* player);
	void draw(SpriteBatch* batch);

	/* Update to play when level finished. */
	void finishedUpdate(double deltaTime);

	vector<Wave*> waves;
private:


};