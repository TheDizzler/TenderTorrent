#include "../pch.h"
#include "PlayerShip.h"
#include "../GameObjects/WeaponSystems/BasicGun.h"
#include "../Engine/GameEngine.h"
#include <random>
#include <algorithm>
//#include "../globals.h"

PlayerShip::PlayerShip() : GameObject() {

}

PlayerShip::~PlayerShip() {

	liveBullets.clear();
	explosions.clear();
	weaponSlots.clear();
}


void PlayerShip::loadShip(xml_node shipNode) {

	load(gfxAssets.getAsset(shipNode.attribute("hull").as_string()));

	speed = shipNode.child("speed").attribute("normal").as_float();
	firingSpeed = shipNode.child("speed").attribute("firing").as_float();

	Vector2 hitpos, hitsize;
	unique_ptr<SubHitArea> subHitArea;
	for (xml_node hitBoxNode : shipNode.child("hitBoxes").children("hitBox")) {

		hitpos = Vector2(hitBoxNode.attribute("x").as_float(), hitBoxNode.attribute("y").as_float());
		hitsize = Vector2(hitBoxNode.attribute("width").as_float(), hitBoxNode.attribute("height").as_float());

		subHitArea.reset(new SubHitArea(hitpos, hitsize));
		subHitAreas.push_back(move(subHitArea));
	}


	Vector2 weaponOffset;
	unique_ptr<WeaponSystem> weaponSlot;
	for (xml_node weaponSlotNode : shipNode.child("weaponSlots").children("weaponSlot")) {
		weaponOffset = Vector2(weaponSlotNode.attribute("x").as_float(),
			weaponSlotNode.attribute("y").as_float());

		enum WeaponType {
			BASIC_GUN, LASERBOLT
		};
		WeaponType weapontype = (WeaponType) weaponSlotNode.attribute("type").as_uint();
		switch (weapontype) {
			case WeaponType::BASIC_GUN:
				weaponSlot.reset(new BasicGun(weaponOffset));
				break;
			case WeaponType::LASERBOLT:
				weaponSlot.reset(new LaserSystem(weaponOffset));
				break;
		}

		weaponSlots.push_back(move(weaponSlot));
	}

	unique_ptr<Mount> mountSlot;
	for (xml_node mountNode : shipNode.child("mounts").children("mount")) {
		weaponOffset = Vector2(
			mountNode.attribute("x").as_float(), mountNode.attribute("y").as_float());

		enum MountType {
			ERROR_, TURRET
		};

		MountType mounttype = (MountType) mountNode.attribute("type").as_uint();
		switch (mounttype) {
			case TURRET:
				mountSlot.reset(new Turret(weaponOffset));
				break;
			case ERROR_:
				GameEngine::errorMessage(L"daaaaamn booooooyyyy", L"Shiiiiet", !Globals::FULL_SCREEN);
				return;

		}

		mounts.push_back(move(mountSlot));
	}

}



void PlayerShip::reset() {

	for each (Bullet* bullet in liveBullets)
		bullet->isAlive = false;
	liveBullets.clear();

	isAlive = true;
	maxEnergy = startMaxEnergy;
	energy = startMaxEnergy;

	for (const auto& slot : weaponSlots)
		slot->update(0, position);

	for (const auto& mount : mounts) {
		mount->setTint(Vector4(1, 1, 1, 1));
		mount->setScale(Vector2(1, 1));
	}

	setScale(Vector2(1, 1));

	setTint(Vector4(1, 1, 1, 1));
	totalDeathTime = 0;
}


void PlayerShip::setPosition(const Vector2& newPosition) {
	Sprite::setPosition(newPosition);

	for (const auto& slot : weaponSlots)
		slot->update(0, position);
	for (const auto& mount : mounts)
		mount->update(0, position);

}

bool PlayerShip::startUpdate(double deltaTime) {

	position.y -= float(firingSpeed * deltaTime);
	for (const auto& mount : mounts)
		mount->update(deltaTime, position);

	return camera.worldToScreen(position).y < camera.viewportHeight - 3 * height;
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

	if (keys.keyState.A || joystick->isLeftPressed())
		position.x -= float(currentSpeed * deltaTime);

	if (keys.keyState.D || joystick->isRightPressed())
		position.x += float(currentSpeed * deltaTime);


	if (keys.keyState.W || joystick->isUpPressed())
		position.y -= float(currentSpeed * deltaTime);
	if (keys.keyState.S || joystick->isDownPressed())
		position.y += float(currentSpeed * deltaTime);

	camera.confineToScreen(this);

	// Weapons
	for (const auto& slot : weaponSlots)
		slot->update(deltaTime, position);


	for (const auto& mount : mounts)
		mount->update(deltaTime, position);

	Vector2 turretsLocationMidPoint = (mounts[0]->getPosition() + mounts[1]->getPosition()) / 2;
	Vector2 mousepos = camera.screenToWorld(mouse.getPosition());
	float y = mousepos.y - turretsLocationMidPoint.y;
	float x = mousepos.x - turretsLocationMidPoint.x;
	Vector2 targetDirection = Vector2(x, y);
	targetDirection.Normalize();

	static_cast<Turret*>(mounts[0].get())->setTurretRotation(targetDirection);
	static_cast<Turret*>(mounts[1].get())->setTurretRotation(targetDirection);

	liveBullets.erase(remove_if(liveBullets.begin(), liveBullets.end(),
		[](const Bullet* bullet) { return !bullet->isAlive; }), liveBullets.end());


	if (keys.keyState.Space || joystick->xButtonDown()) {
		for (const auto& weaponSlot : weaponSlots) {
			if (energy >= weaponSlot->energyCost
				&& weaponSlot->ready()) {
				firing = true;
				liveBullets.push_back(weaponSlot->fire());
				//energy -= weaponSlot->energyCost;
			}
		}
	} else {
		firing = false;
	}

	if (!mouse.leftButtonLast() && mouse.leftButton()) {

		for (const auto& mount : mounts)
			if (mount->ready()) {
				liveBullets.push_back(mount->fire());
				energy -= mount->energyCost;
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

	for (const auto& weaponSlot : weaponSlots)
		weaponSlot->draw(batch);


	Sprite::draw(batch);

	for (const auto& mount : mounts)
		mount->draw(batch);


}

void PlayerShip::finishedUpdate(double deltaTime) {
	// move to center of screen
	Vector2 direction = Vector2(camera.viewportCenter) - position;
	direction.Normalize();
	setPosition(position - direction*speed*float(deltaTime));

}



const double TIME_TO_DIE = 5.0;
const double EXPLOSION_STAGGER_TIME = .2;
void PlayerShip::deathUpdate(double deltaTime) {

	// auto-movement from camera movement
	position -= camera.getDelta();

	Vector2 movement = deathVector * (float) deltaTime;
	position += movement;


	Color newTint = Color::Lerp(Vector4(1, 1, 1, 1), Vector4(0, 0, 0, 0),
		float(totalDeathTime / TIME_TO_DIE));
	setTint(newTint);


	Vector2 newSize = Vector2::Lerp(Vector2(1, 1), Vector2(0, 0),
		float(totalDeathTime / TIME_TO_DIE));
	setScale(newSize);
	for (const auto& mount : mounts) {
		mount->update(deltaTime, position);
		mount->setTint(newTint);
		mount->setScale(newSize);
	}

	totalDeathTime += deltaTime;
	timeSinceLastExplosion -= deltaTime;
	if (timeSinceLastExplosion <= 0) {
		mt19937 rng;
		rng.seed(random_device{}());
		uniform_int_distribution<mt19937::result_type> randx((int) 0, getWidth());
		uniform_int_distribution<mt19937::result_type> randy((int) 0, getHeight());
		Vector2 offset(randx(rng) - getOrigin().x, randy(rng) - getOrigin().y);

		unique_ptr<AnimatedSprite> newExplosion = make_unique<AnimatedSprite>(getPosition() + offset);
		newExplosion->load(gfxAssets.getAnimation("big explosion"));
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


	for (const auto& weaponSlot : weaponSlots)
		weaponSlot->draw(batch);

	Sprite::draw(batch);

	for (const auto& mount : mounts)
		mount->draw(batch);

	for (const auto& explosion : explosions)
		if (explosion->isAlive)
			explosion->draw(batch);
}


bool PlayerShip::checkHitDetection(const HitArea& otherObjHitArea) {

	// first check rough hit box
	if (!hitArea.collision(otherObjHitArea))
		return false;
	for (auto& subHitArea : subHitAreas) {
		// if we update subHitArea positions here we should be saving several cycles per frame
		subHitArea->setPosition(position);
		if (subHitArea->collision(otherObjHitArea))
			return true;
	}
	return false;
}


int PlayerShip::getHealth() {
	return maxEnergy;
}

void PlayerShip::takeDamage(int damageTaken) {

return;

	maxEnergy -= damageTaken;
	if (energy > maxEnergy)
		energy = maxEnergy;
	if (energy <= 0) {
		energy = 0;
		if (!isAlive)
			return;
		isAlive = false;
		// capture direction moving
		deathVector = Vector2::Zero;
		if (keys.keyState.A || joystick->isLeftPressed())
			deathVector.x = -1;
		else if (keys.keyState.D || joystick->isRightPressed())
			deathVector.x = 1;
		if (keys.keyState.W || joystick->isUpPressed())
			deathVector.y = -1;
		else if (keys.keyState.S || joystick->isDownPressed())
			deathVector.y = 1;
		deathVector.Normalize();
		deathVector *= firingSpeed;
	}
}
