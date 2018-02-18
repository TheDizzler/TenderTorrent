#pragma once

#include "Wave.h"
#include "../GameObjects/StarEnemyShip.h"

class StarEnemyShipWave : public Wave {
public:
	
	virtual ~StarEnemyShipWave();
	virtual bool initialize(GFXAssetManager* gfxAssets, xml_node shipNode) override;

	virtual void launchNewWave() override;

protected:

};