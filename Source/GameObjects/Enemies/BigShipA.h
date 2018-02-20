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
	enum BigShipState {
		ENTERING, ATTACK, EXITING
	};

	BigShipState state;

	/* set in xml */
	double timeToTravel;

	
	HexTurret* hexTurretL;
	HexTurret* hexTurretR;
};

