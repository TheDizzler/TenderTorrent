#include "../pch.h"
#pragma once

#include "RearAttackWave.h"
#include "StarEnemyShipWave.h"
#include "ArrowEnemyWave.h"

using namespace std;


class WaveManager {
public:
	WaveManager();
	~WaveManager();

	bool initialize(GFXAssetManager* gfxAssets);
	void clear();

	void clearEnemies();

	void update(double deltaTime, PlayerShip* player);
	void draw(SpriteBatch* batch);

	void finishedUpdate(double deltaTime);

	vector<Wave*> waves;
private:


};