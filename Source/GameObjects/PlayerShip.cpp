#include "PlayerShip.h"


PlayerShip::PlayerShip(const Vector2& pos) : Sprite(pos) {

	position = startPosition;

	rightWeaponSlot.reset(new WeaponSystem(Vector2(26, -15)));
	leftWeaponSlot.reset(new WeaponSystem(Vector2(-26, -15)));
	centerWeaponSlot.reset(new LaserSystem(Vector2(0, 0)));

	weaponSlots.push_back(rightWeaponSlot.get());
	weaponSlots.push_back(leftWeaponSlot.get());
	weaponSlots.push_back(centerWeaponSlot.get());

	rightTurret.reset(new Turret(Vector2(12, -8)));
	leftTurret.reset(new Turret(Vector2(-13, -8)));
}


PlayerShip::~PlayerShip() {

	/*delete rightWeaponSlot;
	delete leftWeaponSlot;
	delete centerWeaponSlot;*/

	liveBullets.clear();
}

void PlayerShip::reset() {

	for each (Bullet* bullet in liveBullets)
		bullet->isAlive = false;
	liveBullets.clear();

	isAlive = true;
	energy = startMaxEnergy;
	position = startPosition;

	rightWeaponSlot->update(0, position);
	leftWeaponSlot->update(0, position);
	centerWeaponSlot->update(0, position);
	rightTurret->update(0, position, Vector2::Zero);
	leftTurret->update(0, position, Vector2::Zero);

}

//void PlayerShip::clear() {
//
//	for each (Bullet* bullet in liveBullets)
//		bullet->isAlive = false;
//	liveBullets.clear();
//
//	position = startPosition;
//}



bool PlayerShip::loadBullet(GFXAssetManager* gfxAsset) {

	rightWeaponSlot->loadBulletTexture(gfxAsset->getAsset("Cross Bullet"));
	leftWeaponSlot->loadBulletTexture(gfxAsset->getAsset("Cross Bullet"));
	centerWeaponSlot->loadBulletTexture(gfxAsset->getAsset("Laserbolt"));

	leftTurret->loadTurretTexture(gfxAsset->getAsset("PlayerShip Turret"));
	rightTurret->loadTurretTexture(gfxAsset->getAsset("PlayerShip Turret"));
	leftTurret->loadBulletTexture(gfxAsset->getAsset("Sun Bullet"));
	rightTurret->loadBulletTexture(gfxAsset->getAsset("Sun Bullet"));

	return true;
}

bool PlayerShip::startUpdate(double deltaTime, MouseController* mouse) {

	position.y -= firingSpeed * deltaTime;
	rightTurret->update(deltaTime, position, mouse->getPosition());
	leftTurret->update(deltaTime, position, mouse->getPosition());
	return position.y < Globals::WINDOW_HEIGHT - 3 * height;
}


#include <algorithm>
void PlayerShip::update(double deltaTime,
	const KeyboardController* keys, MouseController* mouse) {

	if (!isAlive) {

		return;
	}
	// Movement
	int currentSpeed = speed;
	if (firing)
		currentSpeed = firingSpeed;

	auto keyState = Keyboard::Get().GetState();

	//if (keys->keyDown[KeyboardController::LEFT])
	if (keyState.Left)
		position.x -= currentSpeed * deltaTime;

	//if (keys->keyDown[KeyboardController::RIGHT])
	if (keyState.Right)
		position.x += currentSpeed * deltaTime;

	if (position.x < 0 + width / 2)
		position.x = 0 + width / 2;
	if (position.x > Globals::WINDOW_WIDTH - width / 2)
		position.x = Globals::WINDOW_WIDTH - width / 2;

	

	//if (keys->keyDown[KeyboardController::UP])
	if (keyState.Up)
		position.y -= currentSpeed * deltaTime;
	if (keyState.Down)
	//if (keys->keyDown[KeyboardController::DOWN])
		position.y += currentSpeed * deltaTime;

	if (position.y < 0 + height / 2)
		position.y = 0 + height / 2;
	if (position.y > Globals::WINDOW_HEIGHT - height / 2)
		position.y = Globals::WINDOW_HEIGHT - height / 2;

	Sprite::update(deltaTime);

	// Weapons
	rightWeaponSlot->update(deltaTime, position);
	leftWeaponSlot->update(deltaTime, position);
	centerWeaponSlot->update(deltaTime, position);

	rightTurret->update(deltaTime, position, mouse->getPosition());
	leftTurret->update(deltaTime, position, mouse->getPosition());

	liveBullets.erase(remove_if(liveBullets.begin(), liveBullets.end(),
		[](const Sprite* sprite) { return !sprite->isAlive; }), liveBullets.end());



	//if (keys->keyDown[KeyboardController::FIRE]) {
	if (keyState.Space) {
		for (WeaponSystem* weaponSlot : weaponSlots) {
			if (energy >= weaponSlot->energyCost
				&& weaponSlot->ready()) {
				firing = true;
				liveBullets.push_back(weaponSlot->fire());
				energy -= weaponSlot->energyCost;
			}
		}
	} else {
		firing = false;
	}


	//if (!lastStateVKLButtonDown && mouse->leftButtonDown()) {
	if (!mouse->leftButtonLast() && mouse->leftButton()) {

		if (rightTurret->ready()) {
			liveBullets.push_back(rightTurret->fire());
		}
		if (leftTurret->ready()) {
			liveBullets.push_back(leftTurret->fire());
		}
	}

	// Energy recharge
	if (energy < maxEnergy) {
		timeSinceRecharge += deltaTime;

		if (timeSinceRecharge >= rechargeTickCount) {

			energy += rechargeAmount;
			if (energy > maxEnergy)
				energy = maxEnergy;
			timeSinceRecharge = 0;
		}
	} else {

		timeSinceRecharge = 0;
	}

	//lastStateVKLButtonDown = mouse->leftButtonDown();
}

void PlayerShip::draw(SpriteBatch * batch) {

	for (WeaponSystem* weaponSlot : weaponSlots)
		weaponSlot->draw(batch);


	Sprite::draw(batch);

	rightTurret->draw(batch);
	leftTurret->draw(batch);
}

void PlayerShip::takeDamage(int damageTaken) {

	maxEnergy -= damageTaken;
	if (energy > maxEnergy)
		energy = maxEnergy;
	if (energy <= 0) {
		energy = 0;
		isAlive = false;
	}
}
