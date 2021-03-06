#include "../pch.h"
#include "Camera.h"

Camera::Camera() {

	zoom = 1.0f;
	position = Vector2::Zero;
}


Camera::~Camera() {
}

void Camera::setLevel(Background* bgMan) {

	levelWidth = (float) bgMan->getWidth();
	levelHeight = (float) bgMan->getHeight();
}


void Camera::setViewport(D3D11_VIEWPORT cameraViewport) {
	viewportWidth = (int) cameraViewport.Width;
	viewportHeight = (int) cameraViewport.Height;
	viewportCenter = Vector3(viewportWidth * .5f + viewportPosition.x,
		viewportHeight * .5f + viewportPosition.y, 0);
}

void Camera::setViewport(int vwprtWdth, int vwprtHght) {
	viewportWidth = vwprtWdth;
	viewportHeight = vwprtHght;
	viewportCenter = Vector3(viewportWidth * .5f + viewportPosition.x,
		viewportHeight * .5f + viewportPosition.y, 0);
}

void Camera::updateViewport(const Vector2& viewport, const Vector2& viewportPos, bool zoomToFit) {

	viewportWidth = INT(viewport.x);
	viewportHeight = INT(viewport.y);
	viewportPosition = viewportPos;
	viewportCenter = Vector3((viewport.x) * .5f + viewportPosition.x,
		(viewport.y) * .5f + viewportPosition.y, 0);

}

void Camera::confineToScreen(GameObject* obj) {

	Vector2 topleftWorld = screenToWorld(
		Vector2(viewportCenter.x - viewportWidth / 2, viewportCenter.y - viewportHeight / 2));

	Vector2 bottomRightWorld = screenToWorld(
		Vector2(viewportCenter.x + viewportWidth / 2, viewportCenter.y + viewportHeight / 2));

	Vector2 pos = obj->getPosition();
	Vector2 halfsize(float(obj->getWidth()) / 2, float(obj->getHeight()) / 2);
	Vector2 confinedpos = pos;
	if (pos.x - halfsize.x < topleftWorld.x)
		confinedpos.x = topleftWorld.x + halfsize.x;
	else if (pos.x + halfsize.x > bottomRightWorld.x)
		confinedpos.x = bottomRightWorld.x - halfsize.x;
	if (pos.y - halfsize.y < topleftWorld.y)
		confinedpos.y = topleftWorld.y + halfsize.y;
	else if (pos.y + halfsize.y > bottomRightWorld.y)
		confinedpos.y = bottomRightWorld.y - halfsize.y;

	obj->setPosition(confinedpos);

}

bool Camera::viewContains(const Vector2& point) {

	Vector2 screenPos = worldToScreen(point);

	return screenPos.x > viewportPosition.x
		&& screenPos.y > viewportPosition.y
		&& screenPos.x < viewportWidth + viewportPosition.x
		&& screenPos.y < viewportHeight + viewportPosition.y;
}

const Vector2& Camera::getPosition() const {
	return position;
}

const Vector2 & Camera::getDelta() const {
	return positionDelta;
}

float Camera::getZoom() {
	return zoom;
}

void Camera::setZoomToResolution(int width, int height) {
	float xZoom = FLOAT(Globals::WINDOW_WIDTH) / FLOAT(width);
	float yZoom = FLOAT(Globals::WINDOW_HEIGHT) / FLOAT(height);

	if (xZoom < yZoom)
		zoom = xZoom;
	else
		zoom = yZoom;
}

void Camera::setZoom(float zoomTo) {
	zoom = zoomTo;
	if (zoom < 0.2f)
		zoom = 0.2f;
	else if (zoom > 2.5f)
		zoom = 2.5;
}

void Camera::adjustZoom(float amount) {

	zoom += amount;
	if (zoom < 0.2f)
		zoom = 0.2f;
	else if (zoom > 2.5f)
		zoom = 2.5;

}



void Camera::moveCamera(const Vector2& cameraMovement) {

	position += cameraMovement;

}


void Camera::centerOn(const Vector2& pos/*, bool showWholeLevel*/) {

	positionDelta = position - pos;
	/*if (showWholeLevel) {
		zoomToFitBackground();

	}*/

	position = pos;
}

Vector2 Camera::worldToScreen(Vector2 worldPosition) {
	return Vector2::Transform(worldPosition, translationMatrix());
}

Vector2 Camera::screenToWorld(Vector2 screenPosition) {
	return Vector2::Transform(screenPosition, translationMatrix().Invert());
}


Matrix Camera::translationMatrix() {
	// casting to int in CreateTranslation prevents filtering artifacts??
	return Matrix::CreateTranslation(-/*(int)*/ position.x, -/*(int)*/ position.y, 0)
		* Matrix::CreateRotationZ(rotation)
		* Matrix::CreateScale(zoom, zoom, 1)
		* Matrix::CreateTranslation(viewportCenter);
}
