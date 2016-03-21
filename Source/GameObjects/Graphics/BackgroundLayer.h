#pragma once

//#include "Sprite.h"
#include "../PlayerShip.h"
#include "../GUIObjects/TextLabel.h"


class BackgroundLayer : public Sprite {
public:
	BackgroundLayer();
	~BackgroundLayer();

	void setHitArea(const Vector2& position, const Vector2& size, FontSet* font);
	
	void setInitialPosition(const Vector2& position, const Vector2& scale);
	void takeDamage(int damageTaken);

	virtual void setPosition(const Vector2& position) override;
	virtual void draw(SpriteBatch* batch) override;
	
private:

	int health = 200;

	unique_ptr<TextLabel> healthLabel;

	bool labelHidden = true;
};