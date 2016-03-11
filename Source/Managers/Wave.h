#pragma once
#include <vector>

#include "../GameObjects/EnemyShip.h"


using namespace::std;


class Wave {
public:
	Wave();
	~Wave();

	virtual bool initialize(ID3D11Device* device) = 0;
	virtual void launchNewWave() = 0;

	/** Updates ships only. Removes dead enemy bullets. */
	void update(double deltaTime, PlayerShip* player);
	/** Draws both ships and enemy bullets. */
	void draw(SpriteBatch* batch);


	vector<EnemyShip*> enemyShips;
	vector<Bullet*> bullets;
protected:

	unique_ptr<Sprite> sharedShipSprite;
	unique_ptr<Sprite> sharedBulletSprite;

	double timeSinceLastLaunch = 0;
	int maxTimeBetweenLaunches = 15;
private:


};