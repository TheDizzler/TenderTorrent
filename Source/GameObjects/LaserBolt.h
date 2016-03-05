#pragma once

#include "Bullet.h"

class LaserBolt : public Bullet {
public:
	/** For constructing base bullet sprites */
	LaserBolt();
	/** For constructing actual bullets seen on stage. */
	LaserBolt(Vector2 &position);
	~LaserBolt();


	
private:



};