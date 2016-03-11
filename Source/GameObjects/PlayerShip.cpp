#include "PlayerShip.h"


PlayerShip::PlayerShip(const Vector2& pos) : Sprite(pos) {


	rightWeaponSlot = new WeaponSystem(Vector2(position.x + width / 2, position.y));
	leftWeaponSlot = new WeaponSystem(Vector2(position.x - width / 2, position.y));
	centerWeaponSlot = new LaserSystem(Vector2(position.x, position.y));

	weaponSlots.push_back(rightWeaponSlot);
	weaponSlots.push_back(leftWeaponSlot);
	weaponSlots.push_back(centerWeaponSlot);
		/*gunLocationRight = Vector2(position.x + width / 2, position.y);
		gunLocationLeft = Vector2(position.x - width / 2, position.y);
		gunLocationCenter = Vector2(position.x, position.y);*/
}


PlayerShip::~PlayerShip() {
}



bool PlayerShip::loadBullet(ID3D11Device * device) {


	if (!rightWeaponSlot->loadWeaponTexture(device, L"assets/cross bullet.dds")
		|| !leftWeaponSlot->loadWeaponTexture(device, L"assets/cross bullet.dds")
		|| !centerWeaponSlot->loadWeaponTexture(device, L"assets/laserbolt(24x24).dds"))
		return false;

	return true;
}


#include <algorithm>
void PlayerShip::update(double deltaTime, const BYTE keyboardState[256], MouseController* mouse) {

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
	rightWeaponSlot->update(deltaTime, Vector2(position.x + 26, position.y - 15));
	leftWeaponSlot->update(deltaTime, Vector2(position.x - 26, position.y - 15));
	centerWeaponSlot->update(deltaTime, Vector2(position.x, position.y));

	liveBullets.erase(remove_if(liveBullets.begin(), liveBullets.end(),
		[](const Sprite* sprite) { return !sprite->isAlive; }), liveBullets.end());


	if (keyboardState[DIK_SPACE] & 0x80) {
		for (WeaponSystem* weaponSlot : weaponSlots) {
			if (energy >= weaponSlot->energyCost
				&& weaponSlot->ready()) {

				firing = true;
				liveBullets.push_back(weaponSlot->fire());
				energy -= weaponSlot->energyCost;
			} else
				firing = false;
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

		/*rightWeaponSlot->draw(batch);
		leftWeaponSlot->draw(batch);
		centerWeaponSlot->draw(batch);*/

	Sprite::draw(batch);
}
