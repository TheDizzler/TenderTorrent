#include "../../pch.h"
#include "BigShipA.h"
#include "../../Managers/GameManager.h"
#include "../../Engine/GameEngine.h"


BigShipA::BigShipA(xml_node shipNode) : EnemyShip() {

	xml_node weaponPointsNode = shipNode.child("weaponPoints");
	xml_node weaponSystemsNode = shipNode.parent().child("weaponSystems");


	xml_node weaponNode = weaponPointsNode.child("weapon");
	weaponSystems.push_back(make_unique<HexTurret>(weaponNode, weaponSystemsNode));
	weaponNode = weaponNode.next_sibling();
	weaponSystems.push_back(make_unique<HexTurret>(weaponNode, weaponSystemsNode));

	for (weaponNode = weaponNode.next_sibling(); weaponNode; weaponNode = weaponNode.next_sibling()) {
		weaponSystems.push_back(unique_ptr<EnemyWeaponSystem>(
			new EnemyWeaponSystem(weaponNode, weaponSystemsNode)));
	}

	hexTurretL = static_cast<HexTurret*>(weaponSystems[0].get());
	hexTurretR = static_cast<HexTurret*>(weaponSystems[1].get());

	timeToTravel = shipNode.child("timeToTravel").text().as_double();
	maxHealth = shipNode.child("health").text().as_int();

	TIME_TO_ATTACK = shipNode.child("attackTime").text().as_float();
	attackPosition.y = shipNode.child("attackPosition").text().as_float();
	position = Globals::SHIP_STORE_POSITION;
	health = maxHealth;

}


BigShipA::~BigShipA() {
}

void BigShipA::reset() {
	EnemyShip::reset();
}


void BigShipA::launch() {
	EnemyShip::launch();

	leftMost = Vector2(camera.viewportPosition.x + getWidth() / 2, attackPosition.y);
	rightMost = Vector2(camera.viewportWidth, attackPosition.y);

	state = BigShipState::ENTERING;

	mt19937 rng;
	rng.seed(random_device{}());
	uniform_int_distribution<mt19937::result_type> randStartPos(0, TIME_TO_CROSS_SCREEN * 10);

	timeTraveling = float(randStartPos(rng)) / 10;

	uniform_int_distribution<mt19937::result_type> randDir(0, 1);
	if (randDir(rng) == 0) {
		// travelling left
		startTravel = rightMost;
		endTravel = leftMost;
	} else {
		// travelling right
		startTravel = leftMost;
		endTravel = rightMost;
	}



	startPos.x = Vector2::Lerp(
		startTravel, endTravel,
		timeTraveling / TIME_TO_CROSS_SCREEN).x;
	attackPosition.x = startPos.x;
}


void BigShipA::update(double deltaTime, PlayerShip* player, vector<Bullet*>& liveBullets) {

	timeAlive += deltaTime;
	double percent;
	switch (state) {
		case BigShipState::ENTERING:
			percent = timeAlive / timeToTravel;
			setPosition(camera.screenToWorld(Vector2::Lerp(startPos, attackPosition, (float) percent)));

			hexTurretL->setRotation(player->getPosition());
			hexTurretR->setRotation(player->getPosition());
			hexTurretL->timeSinceFired += deltaTime;
			if (hexTurretL->timeSinceFired >= hexTurretL->fireDelay) {
				hexTurretL->timeSinceFired = 0;
				hexTurretL->fired = true;
				liveBullets.push_back(hexTurretL->launchBullet(player->getPosition()));
				hexTurretR->fired = true;
				liveBullets.push_back(hexTurretR->launchBullet(player->getPosition()));
			}

			if (percent >= 1) {
				state = BigShipState::ATTACK;
			}
			break;
		case BigShipState::ATTACK:

			hexTurretL->setRotation(player->getPosition());
			hexTurretR->setRotation(player->getPosition());
			/*hexTurretL->timeSinceFired += deltaTime;
			if (hexTurretL->timeSinceFired >= hexTurretL->fireDelay) {
				hexTurretL->timeSinceFired = 0;
				liveBullets.push_back(hexTurretL->launchBullet(player->getPosition()));
				liveBullets.push_back(hexTurretR->launchBullet(player->getPosition()));
			}*/

			for (auto const& weapon : weaponSystems) {
				weapon->timeSinceFired += deltaTime;
				//weapon->updatePosition(position);
				if (weapon->timeSinceFired >= weapon->fireDelay) {
					weapon->timeSinceFired = 0;
					Bullet* bullet = weapon->launchBullet(player->getPosition());
					liveBullets.push_back(bullet);
				}
			}


			timeTraveling += deltaTime;
			setPosition(camera.screenToWorld(Vector2::Lerp(
				startTravel, endTravel,
				timeTraveling / TIME_TO_CROSS_SCREEN)));

			if (timeTraveling >= TIME_TO_CROSS_SCREEN) {
				timeTraveling = 0;
				Vector2 temp = startTravel;
				startTravel = endTravel;
				endTravel = temp;
			}

			if (timeAlive > TIME_TO_ATTACK) {
				state = BigShipState::EXITING;
				timeSinceExiting = 0;
				startExit = camera.worldToScreen(position);
				endExit = startExit;
				endExit.y = camera.viewportHeight;
			}
			break;
		case BigShipState::EXITING:

			hexTurretL->setRotation(player->getPosition());
			hexTurretR->setRotation(player->getPosition());
			hexTurretL->timeSinceFired += deltaTime;
			if (hexTurretL->timeSinceFired >= hexTurretL->fireDelay) {
				hexTurretL->timeSinceFired = 0;
				hexTurretL->fired = true;
				liveBullets.push_back(hexTurretL->launchBullet(player->getPosition()));
				hexTurretR->fired = true;
				liveBullets.push_back(hexTurretR->launchBullet(player->getPosition()));
			}
			timeSinceExiting += deltaTime;

			percent = timeSinceExiting / timeToTravel;
			setPosition(camera.screenToWorld(Vector2::Lerp(
				startExit, endExit, percent)));

			if (percent > 1.2) { // give it a bit of time to go away
				reset();
			}

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

void BigShipA::HexTurret::setRotation(const Vector2& targetPosition) {

	Vector2 targetDirection = targetPosition - systemLocation;
	targetDirection.Normalize();

	turretSprite.setRotation(atan2(targetDirection.y, targetDirection.x) + XM_PIDIV2);
	//turretDirection = Vector2(targetDirection.x, targetDirection.y);
	//turretDirection.Normalize();

}
