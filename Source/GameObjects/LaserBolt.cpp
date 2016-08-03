#include "LaserBolt.h"

LaserBolt::LaserBolt() :Bullet() {
}

LaserBolt::LaserBolt(Vector2 &position) : Bullet(position) {

	hitArea.reset(new HitArea(position, Vector2(width, height)));

}

LaserBolt::~LaserBolt() {
}

