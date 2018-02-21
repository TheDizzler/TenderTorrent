#pragma once

#include "Wave.h"
#include "../GameObjects/Enemies/RearAttackShip.h"

class RearAttackWave : public Wave {
public:
	
	virtual ~RearAttackWave();

	virtual bool initialize(GFXAssetManager* gfxAssets, xml_node shipNode) override;
	

	virtual void update(double deltaTime, PlayerShip* player) override;

protected:
	virtual void launchNewWave() override;

private:

	void launchNextMiniWave();

	double timeSinceLastMiniLaunch = 0;
	double timeBetweenMiniWaves = .75;
	int miniWavesLaunched = 0;
	int miniWavesMax = 3;
};