#include "LaserBolt.h"

LaserBolt::LaserBolt() :Bullet() {
}

LaserBolt::LaserBolt(Vector2 &position) : Bullet(position) {

	hitArea = new HitArea(position, Vector2(width, height));

}

LaserBolt::~LaserBolt() {
}

