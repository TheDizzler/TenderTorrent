#include "../../pch.h"
#include "EnemyShip.h"
#include "../../Engine/GameEngine.h"
#include "../../globals.h"


EnemyShip::EnemyShip() : GameObject() {
	isAlive = false;
	setExplosion(gfxAssets.getAnimation("big explosion"));
}

EnemyShip::~EnemyShip() {
	weaponSystems.clear();
}

void EnemyShip::setExplosion(Animation* explos) {
	explosion.reset(new AnimatedSprite(position));
	explosion->load(explos);
	explosion->repeats = false;
}

void EnemyShip::reset() {
	setPosition(Globals::SHIP_STORE_POSITION);
	for (auto const& weapon : weaponSystems)
		weapon->reset(position);
	health = maxHealth;
	timeAlive = 0;
	isAlive = false;
	setTint(Vector4(1, 1, 1, 1));
	explosion->reset();
}

void EnemyShip::launch() {
	isAlive = true;
}


void EnemyShip::explodeUpdate(double deltaTime) {

	explosion->update(deltaTime);
	isExploding = explosion->isAlive;
	setTint(Color::Lerp(getTint(), Vector4(0.0f, 0.0f, 0.0f, 0.0f), 20 * (float) deltaTime));

	if (!isExploding) {
		setPosition(Globals::SHIP_STORE_POSITION);
		reset();
	}

}

void EnemyShip::drawExplosion(SpriteBatch* batch) {
	Sprite::draw(batch);
	explosion->draw(batch);
}


int EnemyShip::getHealth() {
	return health;
}

void EnemyShip::takeDamage(int damageTaken) {
	health -= damageTaken;
	if (health < 0) {
		isAlive = false;
		isExploding = true;
		explosion->setPosition(position);
		//return true;
	}
	//return false;
}


EnemyShip::EnemyWeaponSystem::EnemyWeaponSystem(
	xml_node weaponPointNode, xml_node weaponSystemsNode, bool mirrored) {

	const char_t* weaponTypeName = weaponPointNode.attribute("type").as_string();
	xml_node weaponTypeNode = weaponSystemsNode.find_child_by_attribute("weaponType", "name", weaponTypeName);
	int damage = weaponTypeNode.child("damage").text().as_int();
	int bulletSpeed = weaponTypeNode.child("bulletSpeed").text().as_int();
	const char_t* bulletName = weaponTypeNode.child("sprite").text().as_string();

	Animation* bulletAsset;
	bulletAsset = gfxAssets.getAnimation(bulletName);
	if (bulletAsset == NULL) {
		wostringstream wss;
		wss << "Unable to find weapon asset " << bulletName;
		wss << " in EnemyShip::EnemyWeaponSystem.";
		GameEngine::showErrorDialog(wss.str(), L"This is bad");
		return;
	}
	float angleDeg = weaponPointNode.child("angle").text().as_float();
	float angle = angleDeg * XM_PI / 180;
	Vector2 direction;
	if (mirrored)
		direction = Vector2(1, 1);
	else
		direction = Vector2(-1, 1);
	direction *= Vector2(sin(angle), cos(angle));
	direction.Normalize();
	for (int i = 0; i < MAX_BULLETS_IN_STORE; ++i) {
		unique_ptr<EnemyBullet> bullet = make_unique<EnemyBullet>();
		bullet->damage = damage;
		bullet->bulletSpeed = bulletSpeed;
		bullet->direction = direction;
		bullet->load(bulletAsset);
		bulletStore.push_back(move(bullet));
	}

	fireDelay = weaponPointNode.child("fireRate").attribute("delay").as_float();
	positionOffset = Vector2((float) weaponPointNode.child("location").attribute("x").as_int(),
		(float) weaponPointNode.child("location").attribute("y").as_int());

	targeted = weaponTypeNode.child("targeted").text().as_bool();
}

EnemyShip::EnemyWeaponSystem::~EnemyWeaponSystem() {
	bulletStore.clear();
}


void EnemyShip::EnemyWeaponSystem::reset(const Vector2& shipPosition) {
	systemLocation = shipPosition + positionOffset;
	fired = false;
	timeSinceFired = fireDelay;
	nextBulletInStore = 0;
}

void EnemyShip::EnemyWeaponSystem::updatePosition(const Vector2& shipPosition) {
	systemLocation = shipPosition + positionOffset;
}

bool EnemyShip::EnemyWeaponSystem::ready() {
	if (timeSinceFired < fireDelay)
		return false;
	if (bulletStore[nextBulletInStore]->isAlive == false)
		return true;

	return false;
}

EnemyBullet* EnemyShip::EnemyWeaponSystem::launchBullet(const Vector2& target) {

	if (!targeted)
		return launchBullet();
	else {
		EnemyBullet* bullet = bulletStore[nextBulletInStore++].get();
		bullet->setPosition(systemLocation);
		Vector2 direction = target - systemLocation;
		direction.Normalize();
		bullet->direction = direction;
		bullet->isAlive = true;
		timeSinceFired = 0;

		if (nextBulletInStore >= bulletStore.size())
			nextBulletInStore = 0;
		return bullet;
	}
}

EnemyBullet* EnemyShip::EnemyWeaponSystem::launchBullet() {

	EnemyBullet* bullet = bulletStore[nextBulletInStore++].get();
	bullet->setPosition(systemLocation);
	bullet->isAlive = true;
	timeSinceFired = 0;

	if (nextBulletInStore >= bulletStore.size())
		nextBulletInStore = 0;

	return bullet;
}

