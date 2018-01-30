#include "../../pch.h"
#include "GameObject.h"




GameObject::GameObject() : Sprite() {
}

GameObject::GameObject(const Vector2& pos) : Sprite(pos) {
}

GameObject::~GameObject() {
}
