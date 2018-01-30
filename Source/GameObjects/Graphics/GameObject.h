#pragma once
#include "../../../DXTKGui/BaseGraphics/Sprite.h"


class GameObject : public Sprite {
public:
	GameObject();
	GameObject(const Vector2& position);
	virtual ~GameObject();

	bool isExploding = false;
protected:
};