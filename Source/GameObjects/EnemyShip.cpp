#include "EnemyShip.h"

EnemyShip::EnemyShip() : Sprite(SimpleMath::Vector2(0, 0)) {

	isAlive = false;
}

EnemyShip::EnemyShip(const Vector2& position) : Sprite(position) {

	isAlive = false;
}

EnemyShip::~EnemyShip() {
	weaponSystems.clear();
}

void EnemyShip::reset() {
	position = startPos;
	for (auto const& weapon : weaponSystems)
		weapon->reset(position);
	health = maxHealth;
	timeAlive = 0;
	isAlive = true;
}


void EnemyShip::update(double deltaTime) {
	Sprite::update(deltaTime);
}


void EnemyShip::takeDamage(int damageTaken) {
	health -= damageTaken;
	isAlive = health > 0;
}


#include "../Engine/GameEngine.h"
EnemyShip::EnemyWeaponSystem::EnemyWeaponSystem(xml_node weaponPointNode, xml_node weaponSystemsNode, bool mirrored) {

	const char_t* weaponTypeName = weaponPointNode.attribute("type").as_string();
	xml_node weaponTypeNode = weaponSystemsNode.find_child_by_attribute("weaponType", "name", weaponTypeName);
	int damage = weaponTypeNode.child("damage").text().as_int();
	int bulletSpeed = weaponTypeNode.child("bulletSpeed").text().as_int();
	const char_t* bulletName = weaponTypeNode.child("sprite").text().as_string();
	
	shared_ptr<Animation> bulletAsset;
	bulletAsset = gfxAssets->getAnimation(bulletName);
	if (bulletAsset == NULL) {
		wostringstream wss;
		wss << "Unable to find weapon asset " << bulletName;
		wss << " in EnemyShip::EnemyWeaponSystem.";
		GameEngine::showErrorDialog(wss.str(), L"This is bad");
		return;
	}
	float angle = weaponPointNode.child("angle").text().as_float()*XM_PI / 180;
	Vector2 direction;
	if (mirrored)
		direction = Vector2(1, 1);
	else
		direction = Vector2(-1, 1);
	direction *= Vector2(sin(angle), cos(angle));
	direction.Normalize();
	for (int i = 0; i < MAX_BULLETS_IN_STORE; ++i) {
		EnemyBullet* bullet = new EnemyBullet();
		bullet->damage = damage;
		bullet->bulletSpeed = bulletSpeed;
		bullet->direction = direction;
		bullet->load(bulletAsset);
		bulletStore.push_back(bullet);
	}

	fireDelay = weaponPointNode.child("fireRate").attribute("delay").as_float();
	positionOffset = Vector2(weaponPointNode.child("location").attribute("x").as_int(),
		weaponPointNode.child("location").attribute("y").as_int());
}

EnemyShip::EnemyWeaponSystem::~EnemyWeaponSystem() {
	for each (Bullet* bullet in bulletStore)
		delete bullet;
	bulletStore.clear();
}

void EnemyShip::EnemyWeaponSystem::reset(const Vector2 & shipPosition) {
	systemLocation = shipPosition + positionOffset;
	fired = false;
}

void EnemyShip::EnemyWeaponSystem::updatePosition(Vector2 shipPosition) {
	systemLocation = shipPosition + positionOffset;
}

EnemyBullet* EnemyShip::EnemyWeaponSystem::launchBullet(Vector2 target) {

	EnemyBullet* bullet = bulletStore[nextBulletInStore++];
	bullet->setPosition(systemLocation);
	Vector2 direction = target - systemLocation;
	direction.Normalize();
	bullet->direction = direction;
	bullet->isAlive = true;

	if (nextBulletInStore >= bulletStore.size())
		nextBulletInStore = 0;
	return bullet;
}

EnemyBullet * EnemyShip::EnemyWeaponSystem::launchBullet() {

	EnemyBullet* bullet = bulletStore[nextBulletInStore++];
	bullet->setPosition(systemLocation);
	bullet->isAlive = true;

	if (nextBulletInStore >= bulletStore.size())
		nextBulletInStore = 0;

	return bullet;
}

