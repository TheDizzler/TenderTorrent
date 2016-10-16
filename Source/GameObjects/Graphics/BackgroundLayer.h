#include "../../pch.h"
#pragma once

#include "../PlayerShip.h"
#include "../../DXTKGui/Controls/TextLabel.h"


class BackgroundLayer : public Sprite {
public:
	BackgroundLayer();
	~BackgroundLayer();

	void setHitArea(const Vector2& position, const Vector2& size);

	void setInitialPosition(const Vector2& position, const Vector2& scale);
	void takeDamage(int damageTaken);

	virtual void setPosition(const Vector2& position) override;
	void draw(SpriteBatch* batch, Sprite* frame);

private:

	Vector2 topLeftCornerPos, topRightCornerPos,
		bottomLeftCornerPos, bottomRightCornerPos;

	int health = 200;

	unique_ptr<TextLabel> healthLabel;

	bool labelHidden = true;
};