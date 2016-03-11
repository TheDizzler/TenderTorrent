#pragma once

#include "RearAttackWave.h"


using namespace std;


class WaveManager {
public:
	WaveManager();
	~WaveManager();

	bool initialize(ID3D11Device* device);

	void update(double deltaTime, PlayerShip* player);
	void draw(SpriteBatch* batch);

	//vector<EnemyBullet*> bullets;
	//vector<RearAttackShip*> enemyShips;
	vector<Wave*> waves;
private:

	/*unique_ptr<Sprite> sharedEnemyShipA;
	unique_ptr<Sprite> sharedEnemyShipB;


	unique_ptr<Sprite> yellowBullet;*/
	

};