#include "../pch.h"
#include "PlayerShip.h"
//#include "../Managers/GameManager.h"
#include "../Engine/GameEngine.h"
#include <random>
#include <algorithm>


PlayerShip::PlayerShip() : GameObject() {


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

	liveBullets.clear();
	explosions.clear();
}

void PlayerShip::reset() {

	for each (Bullet* bullet in liveBullets)
		bullet->isAlive = false;
	liveBullets.clear();

	isAlive = true;
	maxEnergy = startMaxEnergy;
	energy = startMaxEnergy;
	//position = PLAYER_START_POSITION;

	rightWeaponSlot->update(0, position);
	leftWeaponSlot->update(0, position);
	centerWeaponSlot->update(0, position);
	rightTurret->update(0, position);
	leftTurret->update(0, position);

	setTint(Vector4(1, 1, 1, 1));
	totalDeathTime = 0;
}



bool PlayerShip::loadBullet(GFXAssetManager* gfxAsset) {

	rightWeaponSlot->loadBulletTexture(gfxAsset->getAnimation("Cross Bullet"));
	leftWeaponSlot->loadBulletTexture(gfxAsset->getAnimation("Cross Bullet"));
	centerWeaponSlot->loadBulletTexture(gfxAsset->getAnimation("Laserbolt"));

	leftTurret->loadTurretTexture(gfxAsset->getAsset("PlayerShip Turret"));
	rightTurret->loadTurretTexture(gfxAsset->getAsset("PlayerShip Turret"));
	leftTurret->loadBulletTexture(gfxAsset->getAnimation("Sun Bullet"));
	rightTurret->loadBulletTexture(gfxAsset->getAnimation("Sun Bullet"));

	return true;
}

bool PlayerShip::startUpdate(double deltaTime) {

	position.y -= float(firingSpeed * deltaTime);
	rightTurret->update(deltaTime, position);
	leftTurret->update(deltaTime, position);
	//return position.y < Globals::WINDOW_HEIGHT - 3 * height;
	return camera.worldToScreen(position).y < Globals::WINDOW_HEIGHT - 3 * height;
}



void PlayerShip::update(double deltaTime) {

	if (!isAlive) {
		return;
	}

	// auto-movement from camera movement
	position -= camera.getDelta();

	// Movement
	float currentSpeed = speed;
	if (firing)
		currentSpeed = firingSpeed;

	auto keyState = Keyboard::Get().GetState();

	if (keyState.A)
		position.x -= float(currentSpeed * deltaTime);

	if (keyState.D)
		position.x += float(currentSpeed * deltaTime);

	/*if (position.x < width / 2)
		position.x = float(width) / 2;
	if (position.x > Globals::WINDOW_WIDTH - width / 2)
		position.x = Globals::WINDOW_WIDTH - float(width) / 2;*/


	if (keyState.W)
		position.y -= float(currentSpeed * deltaTime);
	if (keyState.S)
		position.y += float(currentSpeed * deltaTime);

	/*if (position.y < height / 2)
		position.y = float(height) / 2;
	if (position.y > Globals::WINDOW_HEIGHT - height / 2)
		position.y = Globals::WINDOW_HEIGHT - float(height) / 2;*/

	// Weapons
	rightWeaponSlot->update(deltaTime, position);
	leftWeaponSlot->update(deltaTime, position);
	centerWeaponSlot->update(deltaTime, position);

	rightTurret->update(deltaTime, position);
	leftTurret->update(deltaTime, position);
	Vector2 turretsLocationMidPoint = (rightTurret->getPosition() + leftTurret->getPosition()) / 2;
	Vector2 mousepos = camera.screenToWorld(mouse.getPosition());
	float y = mousepos.y - turretsLocationMidPoint.y;
	float x = mousepos.x - turretsLocationMidPoint.x;
	Vector2 targetDirection = Vector2(x, y);
	targetDirection.Normalize();

	rightTurret->setTurretRotation(targetDirection);
	leftTurret->setTurretRotation(targetDirection);

	liveBullets.erase(remove_if(liveBullets.begin(), liveBullets.end(),
		[](const Bullet* bullet) { return !bullet->isAlive; }), liveBullets.end());


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
	if (!mouse.leftButtonLast() && mouse.leftButton()) {

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
}


void PlayerShip::draw(SpriteBatch * batch) {

	for (WeaponSystem* weaponSlot : weaponSlots)
		weaponSlot->draw(batch);


	Sprite::draw(batch);

	rightTurret->draw(batch);
	leftTurret->draw(batch);

}

void PlayerShip::finishedUpdate(double deltaTime) {

// move to center of screen
//position += Vector2::Lerp( levelEndPosition, centerPosition,  
}



const double TIME_TO_DIE = 5.0;
const double EXPLOSION_STAGGER_TIME = .2;
void PlayerShip::deathUpdate(double deltaTime) {

	Vector2 movement = deathVector * (float) deltaTime;
	position += movement;
	rightTurret->update(deltaTime, position);
	leftTurret->update(deltaTime, position);

	Color newTint = Color::Lerp(Vector4(1, 1, 1, 1), Vector4(0, 0, 0, 0),
		float(totalDeathTime / TIME_TO_DIE));
	setTint(newTint);
	rightTurret->setTint(newTint);
	leftTurret->setTint(newTint);

	Vector2 newSize = Vector2::Lerp(Vector2(1, 1), Vector2(0, 0),
		float(totalDeathTime / TIME_TO_DIE));
	setScale(newSize);
	rightTurret->setScale(newSize);
	leftTurret->setScale(newSize);

	totalDeathTime += deltaTime;
	timeSinceLastExplosion -= deltaTime;
	if (timeSinceLastExplosion <= 0) {
		mt19937 rng;
		rng.seed(random_device{}());
		uniform_int_distribution<mt19937::result_type> randx((int) 0, getWidth());
		uniform_int_distribution<mt19937::result_type> randy((int) 0, getHeight());
		Vector2 offset(randx(rng) - getOrigin().x, randy(rng) - getOrigin().y);

		unique_ptr<AnimatedSprite> newExplosion = make_unique<AnimatedSprite>(getPosition() + offset);
		newExplosion->load(gfxAssets->getAnimation("big explosion"));
		newExplosion->repeats = false;
		explosions.push_back(move(newExplosion));

		timeSinceLastExplosion = EXPLOSION_STAGGER_TIME;
	}

	for (const auto& explosion : explosions) {
		if (explosion->isAlive) {
			explosion->moveBy(movement);
			explosion->update(deltaTime);
		}
	}


}


void PlayerShip::deathDraw(SpriteBatch* batch) {


	for (WeaponSystem* weaponSlot : weaponSlots)
		weaponSlot->draw(batch);

	Sprite::draw(batch);

	rightTurret->draw(batch);
	leftTurret->draw(batch);

	for (const auto& explosion : explosions)
		if (explosion->isAlive)
			explosion->draw(batch);
}

int PlayerShip::getHealth() {
	return maxEnergy;
}

void PlayerShip::takeDamage(int damageTaken) {

	maxEnergy -= damageTaken;
	if (energy > maxEnergy)
		energy = maxEnergy;
	if (energy <= 0) {
		energy = 0;
		if (!isAlive)
			return;
		isAlive = false;
		// capture direction moving
		auto keyState = Keyboard::Get().GetState();
		deathVector = Vector2::Zero;
		if (keyState.A)
			deathVector.x = -1;
		else if (keyState.D)
			deathVector.x = 1;
		if (keyState.W)
			deathVector.y = -1;
		else if (keyState.S)
			deathVector.y = 1;
		deathVector.Normalize();
		deathVector *= firingSpeed;
	}
}
