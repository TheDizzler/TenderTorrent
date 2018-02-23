#pragma once

#include "WeaponSystem.h"

class Mount : public WeaponSystem {
public:
	Mount(Vector2 locationOffset);
	virtual ~Mount();

	virtual void setScale(const Vector2& newScale) = 0;
};
