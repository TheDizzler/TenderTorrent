#pragma once


#include "AnimatedSprite.h"

/* A graphical attachment to a ship (ex: engines). Generally this is eyecandy only. */
class Attachment : public AnimatedSprite {
public:
	virtual ~Attachment();

	void setOffset(const Vector2& positionOffset);

	void updatePosition(const Vector2& shipPosition);

private:
	/* Location of attachment relative to ship attached to. */
	Vector2 positionOffset;
	/* World space coords of attachment. */
	Vector2 attachmentLocation;
};
