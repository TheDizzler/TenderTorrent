#pragma once

#include "RearAttackWave.h"
#include "StarEnemyShipWave.h"

using namespace std;


class WaveManager {
public:
	WaveManager();
	~WaveManager();

	bool initialize(GFXAssetManager* gfxAssets);
	void clear();

	void update(double deltaTime, PlayerShip* player);
	void draw(SpriteBatch* batch);


	vector<Wave*> waves;
private:


};