#pragma once

#include "Wave.h"
#include "../GameObjects/StarEnemyShip.h"

class StarEnemyShipWave : public Wave {
public:
	StarEnemyShipWave();
	~StarEnemyShipWave();
	virtual bool initialize(GFXAssetManager* gfxAssets) override;

	virtual void launchNewWave() override;

protected:
	virtual bool checkForLaunch() override;

private:

};