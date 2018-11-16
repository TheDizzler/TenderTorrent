#include "../../pch.h"
#include "LaserBolt.h"
#include "../../Engine/GameEngine.h"

LaserBolt::LaserBolt() : Bullet() {
	load(gfxAssets.getAnimation("Laserbolt"));
	bulletSpeed = 750;
}

LaserBolt::~LaserBolt() {
}

