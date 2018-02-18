#pragma once
#include "../../../DXTKGui/BaseGraphics/Sprite.h"


class GameObject : public Sprite {
public:
	GameObject();
	virtual ~GameObject();

	bool isExploding = false;
protected:
};