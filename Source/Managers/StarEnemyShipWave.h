#pragma once

#include "Wave.h"
#include "../GameObjects/Enemies/StarEnemyShip.h"

class StarEnemyShipWave : public Wave {
public:
	
	virtual ~StarEnemyShipWave();
	virtual bool initialize(GFXAssetManager* gfxAssets, xml_node shipNode) override;

	virtual void launchNewWave() override;

protected:

};