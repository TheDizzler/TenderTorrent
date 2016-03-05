#pragma once
#include <vector>

#include "Wave.h"
#include "../GameObjects/RearAttackShip.h"

using namespace::std;


class RearAttackWave : public Wave {
public:
	RearAttackWave();
	~RearAttackWave();

	virtual bool initialize(ID3D11Device* device);
	virtual void launchNewWave();


private:

	
};