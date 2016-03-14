#include "PlayerShip.h"


PlayerShip::PlayerShip(const Vector2& pos) : Sprite(pos) {

	position = startPosition;

	rightWeaponSlot = new WeaponSystem(Vector2(26, -15));
	leftWeaponSlot = new WeaponSystem(Vector2(-26, -15));
	centerWeaponSlot = new LaserSystem(Vector2(0, 0));

	weaponSlots.push_back(rightWeaponSlot);
	weaponSlots.push_back(leftWeaponSlot);
	weaponSlots.push_back(centerWeaponSlot);

	rightTurret.reset(new Turret(Vector2(12, -8)));
	leftTurret.reset(new Turret(Vector2(-13, -8)));
}


PlayerShip::~PlayerShip() {

	delete rightWeaponSlot;
	delete leftWeaponSlot;
	delete centerWeaponSlot;
}



bool PlayerShip::loadBullet(ID3D11Device * device) {


	if (!rightWeaponSlot->loadBulletTexture(device, L"assets/cross bullet.dds")
		|| !leftWeaponSlot->loadBulletTexture(device, L"assets/cross bullet.dds")
		|| !centerWeaponSlot->loadBulletTexture(device, L"assets/laserbolt(24x24).dds"))
		return false;

	if (!leftTurret->loadTurretTexture(device, L"assets/Turret(24x24).dds")
		|| !rightTurret->loadTurretTexture(device, L"assets/Turret(24x24).dds")
		|| !leftTurret->loadBulletTexture(device, L"assets/cross bullet.dds")
		|| !rightTurret->loadBulletTexture(device, L"assets/cross bullet.dds"))
		return false;

	return true;
}

bool PlayerShip::startUpdate(double deltaTime, MouseController* mouse) {

	position.y -= firingSpeed * deltaTime;
	rightTurret->update(deltaTime, position, mouse->position);
	leftTurret->update(deltaTime, position, mouse->position);
	return position.y < Globals::WINDOW_HEIGHT - 3 * height;
}


#include <algorithm>
void PlayerShip::update(double deltaTime, const BYTE keyboardState[256], MouseController* mouse) {

	if (!isAlive) {

		return;
	}
		// Movement
	int currentSpeed = speed;
	if (firing)
		currentSpeed = firingSpeed;

	if (keyboardState[DIK_A] & 0x80)
		position.x -= currentSpeed * deltaTime;
	if (keyboardState[DIK_D] & 0x80)
		position.x += currentSpeed * deltaTime;

	if (position.x < 0 + width / 2)
		position.x = 0 + width / 2;
	if (position.x > Globals::WINDOW_WIDTH - width / 2)
		position.x = Globals::WINDOW_WIDTH - width / 2;

	if (keyboardState[DIK_W] & 0x80)
		position.y -= currentSpeed * deltaTime;
	if (keyboardState[DIK_S] & 0x80)
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

	rightTurret->update(deltaTime, position, mouse->position);
	leftTurret->update(deltaTime, position, mouse->position);

	liveBullets.erase(remove_if(liveBullets.begin(), liveBullets.end(),
		[](const Sprite* sprite) { return !sprite->isAlive; }), liveBullets.end());


	if (keyboardState[DIK_SPACE] & 0x80) {
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
