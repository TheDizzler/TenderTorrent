#include "../../pch.h"
#include "LaserBolt.h"
#include "../../Engine/GameEngine.h"

LaserBolt::LaserBolt() : Bullet() {
	load(gfxAssets.getAnimation("Laserbolt"));
}

//LaserBolt::LaserBolt(Vector2 &position) : Bullet(position) {
//
//}

LaserBolt::~LaserBolt() {
}

