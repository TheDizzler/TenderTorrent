#pragma once
#include "EnemyShip.h"


class BigShipA : public EnemyShip {
protected:
	class HexTurret : public EnemyWeaponSystem {
	public:
		HexTurret(xml_node weaponNode, xml_node weaponSystemsNode);
		virtual ~HexTurret();

		virtual void draw(SpriteBatch* batch);

		virtual void updatePosition(const Vector2& shipPosition) override;

		virtual void setRotation(const Vector2& targetPosition);
	protected:
		Sprite turretSprite;
	};

public:
	BigShipA(xml_node shipNode);
	virtual ~BigShipA();

	virtual void reset() override;
	virtual void launch() override;

	virtual void update(double deltaTime, PlayerShip* player, vector<Bullet*>& liveBullets) override;
	virtual void draw(SpriteBatch* batch) override;

	virtual void setPosition(const Vector2& newPosition) override;

private:
	Attachment engineL;
	Attachment engineR;

	enum BigShipState {
		ENTERING, ATTACK, EXITING
	};

	BigShipState state;

	/* Time ship stays in ATTACK state...almost. Actually includes the ENTERING time. */
	float TIME_TO_ATTACK;


	/** Position where BigShip stops moving forward and commences it's attack pattern. */
	Vector2 attackPosition = Vector2::Zero;
	/* Time to reach AttackPosition set in xml */
	double timeToTravel;
	double timeTraveling = 0;
	double timeSinceExiting;

	float TIME_TO_CROSS_SCREEN = 5;
	Vector2 leftMost;
	Vector2 rightMost;
	Vector2 startTravel, endTravel;
	Vector2 startExit, endExit;

	
	HexTurret* hexTurretL;
	HexTurret* hexTurretR;
};

