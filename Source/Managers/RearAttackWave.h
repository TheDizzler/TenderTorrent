#include "../pch.h"
#pragma once

#include "Wave.h"
#include "../GameObjects/RearAttackShip.h"



class RearAttackWave : public Wave {
public:
	RearAttackWave();
	~RearAttackWave();

	virtual bool initialize(GFXAssetManager* gfxAssets) override;
	virtual void launchNewWave() override;

	virtual void update(double deltaTime, PlayerShip* player) override;

protected:
	virtual bool checkForLaunch() override;
private:

	void launchNextMiniWave();

	double timeSinceLastMiniLaunch = 0;
	double timeBetweenMiniWaves = .75;
	int miniWavesLaunched = 0;
	int miniWavesMax = 3;
};