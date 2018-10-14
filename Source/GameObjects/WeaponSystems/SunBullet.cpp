#include "../../pch.h"
#include "SunBullet.h"
#include "../../globals.h"
#include "../../Engine/GameEngine.h"


SunBullet::SunBullet() : Bullet() {
	load(gfxAssets.getAnimation("Sun Bullet"));
	bulletSpeed = 550;
}


SunBullet::~SunBullet() {
}


void SunBullet::update(double deltaTime) {

	Bullet::update(deltaTime);

}
