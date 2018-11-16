#include "FlameWave.h"
#include "../../Engine/GameEngine.h"

FlameWave::FlameWave() {
	load(gfxAssets.getAnimation("Flame Wave"));
	bulletSpeed = 750;
}

FlameWave::~FlameWave() {
}
