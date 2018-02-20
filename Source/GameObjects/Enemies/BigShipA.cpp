#include "../../pch.h"
#include "BigShipA.h"
#include "../../Managers/GameManager.h"
#include "../../Engine/GameEngine.h"


BigShipA::BigShipA(xml_node shipNode) : EnemyShip() {

	xml_node weaponPointsNode = shipNode.child("weaponPoints");
	xml_node weaponSystemsNode = shipNode.parent().child("weaponSystems");


	for (xml_node weaponNode = weaponPointsNode.child("weapon");
		weaponNode; weaponNode = weaponNode.next_sibling()) {

		weaponSystems.push_back(make_unique<HexTurret>(weaponNode, weaponSystemsNode));
	}
	hexTurretL = static_cast<HexTurret*>(weaponSystems[0].get());
	hexTurretR = static_cast<HexTurret*>(weaponSystems[1].get());

	timeToTravel = shipNode.child("timeToTravel").text().as_double();
	maxHealth = shipNode.child("health").text().as_int();


	position = Globals::SHIP_STORE_POSITION;
	health = maxHealth;
}


BigShipA::~BigShipA() {
}

void BigShipA::reset() {
	EnemyShip::reset();
}

Vector2 attackPosition(0, 250);
void BigShipA::launch() {
	EnemyShip::launch();
	state = BigShipState::ENTERING;

	mt19937 rng;
	rng.seed(random_device{}());
	uniform_int_distribution<mt19937::result_type> rand(0, camera.viewportWidth);
	startPos = Vector2((float) rand(rng), 0);
	attackPosition.x = startPos.x;
}


void BigShipA::update(double deltaTime, PlayerShip* player, vector<Bullet*>& liveBullets) {

	timeAlive += deltaTime;
	double percent = timeAlive / timeToTravel;
	switch (state) {
		case BigShipState::ENTERING:

			setPosition(camera.screenToWorld(Vector2::Lerp(startPos, attackPosition, (float) percent)));

			if (percent >= 1)
				state = BigShipState::ATTACK;
			break;
		case BigShipState::ATTACK:


			break;
		case BigShipState::EXITING:

			break;
	}
}

void BigShipA::draw(SpriteBatch* batch) {
	EnemyShip::draw(batch);
	hexTurretL->draw(batch);
	hexTurretR->draw(batch);
}

void BigShipA::setPosition(const Vector2& newPosition) {
	EnemyShip::setPosition(newPosition);
	for (auto const& weapon : weaponSystems) {
		weapon->updatePosition(position);
	}
}



BigShipA::HexTurret::HexTurret(xml_node weaponNode, xml_node weaponSystemsNode)
	: EnemyWeaponSystem(weaponNode, weaponSystemsNode) {

	const char_t* spritename = weaponNode.child("sprite").attribute("name").as_string();

	turretSprite.load(gfxAssets.getAsset(spritename));
	positionOffset = Vector2(positionOffset.x + turretSprite.getOrigin().x,
		positionOffset.y + 8);
	//turretSprite.setPosition(systemLocation);
}

BigShipA::HexTurret::~HexTurret() {
}


void BigShipA::HexTurret::draw(SpriteBatch* batch) {
	turretSprite.draw(batch);
}

void BigShipA::HexTurret::updatePosition(const Vector2& shipPosition) {
	EnemyWeaponSystem::updatePosition(shipPosition);
	turretSprite.setPosition(systemLocation);
}
