#pragma once
#include "Wave.h"

class BigShipAWave : public Wave {
public:

	virtual ~BigShipAWave();

	virtual bool initialize(GFXAssetManager * gfxAssets, xml_node shipNode) override;


	virtual void update(double deltaTime, PlayerShip* player) override;

protected:
	virtual void launchNewWave() override;
};
