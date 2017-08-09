#include "../pch.h"
#include "Camera.h"

Camera::Camera(int vwprtWdth, int vwprtHght) {

	zoom = 1.0f;

	viewportWidth = vwprtWdth;
	viewportHeight = vwprtHght;
	viewportCenter = Vector3(viewportWidth * .5, viewportHeight * .5, 0);


	//viewX = (viewportWidth) / zoom / 2;
	//viewY = (viewportHeight) / zoom / 2;

	cameraPosition = Vector2::Zero;
}


Camera::~Camera() {
}

void Camera::setLevel(Background* bgMan) {

	levelWidth = bgMan->getWidth();
	levelHeight = bgMan->getHeight();
}


void Camera::updateViewport(const Vector2& viewport, const Vector2& viewportPos, bool zoomToFit) {

	viewportWidth = viewport.x - viewportPos.x;
	viewportHeight = viewport.y - viewportPos.y;
	viewportPosition = viewportPos;
	viewportCenter = Vector3((viewportWidth) * .5 + viewportPosition.x,
		(viewportHeight) * .5 + viewportPosition.y, 0);

	//viewX = (viewportWidth) / zoom / 2;
	//viewY = (viewportHeight) / zoom / 2;
	//if (zoomToFit)
		//zoomToFitBuilding();
}

bool Camera::viewContains(const Vector2& point) {
	RECT* rect = viewportWorldBoundary();

	bool contains = rect->left < point.x && point.x < rect->right
		&& rect->top < point.y && point.y < rect->bottom;
	delete rect;
	return contains;
}

float Camera::getZoom() {
	return zoom;
}

void Camera::setZoomToResolution(int width, int height) {
	float xZoom = Globals::WINDOW_WIDTH / width;
	float yZoom = Globals::WINDOW_HEIGHT / height;

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

	//viewX = (viewportWidth / zoom / 2);
	//viewY = (viewportHeight / zoom / 2);
}

void Camera::adjustZoom(float amount) {

	zoom += amount;
	if (zoom < 0.2f)
		zoom = 0.2f;
	else if (zoom > 2.5f)
		zoom = 2.5;

	//viewX = (viewportWidth / zoom / 2);
	//viewY = (viewportHeight / zoom / 2);
}



void Camera::moveCamera(const Vector2& cameraMovement) {

	cameraPosition += cameraMovement;

}


void Camera::setCameraPosition(const Vector2& newPosition) {

	cameraPosition = newPosition;
	/*if (position.x < 0)
	position.x = 0;
	if (position.x > levelWidth)
	position.x = levelWidth;*/
}

RECT* Camera::viewportWorldBoundary() {

	/*Vector2* viewportCorner = screenToWorld(Vector2::Zero);
	Vector2* viewportBottomCorner =
		screenToWorld(Vector2(viewportWidth, viewportHeight));*/

	RECT* rect = new RECT{
		(int) viewportPosition.x, (int) viewportPosition.y,
		(int) (viewportWidth - viewportPosition.x),
		(int) (viewportHeight - viewportPosition.y)};

	//delete viewportCorner;
	//delete viewportBottomCorner;

	return rect;
}


void Camera::centerOn(const Vector2& pos/*, bool showWholeLevel*/) {

	/*if (showWholeLevel) {
		zoomToFitBackground();

	}*/

	cameraPosition = pos;
}

Vector2& Camera::worldToScreen(Vector2 worldPosition) {
	return Vector2::Transform(worldPosition, translationMatrix());
}

Vector2& Camera::screenToWorld(Vector2 screenPosition) {
	return Vector2::Transform(screenPosition, translationMatrix().Invert());
}



//void Camera::zoomToFitBackground() {
//	int widthDif = levelWidth - viewX;
//	int heightDif = levelHeight - viewY;
//	if (widthDif > heightDif) {
//		zoom = (viewportWidth / levelWidth);
//		viewX = (viewportWidth / zoom / 2);
//		viewY = (viewportHeight / zoom / 2);
//
//	} else {
//		zoom = (viewportHeight / levelHeight);
//		viewX = (viewportWidth / zoom / 2);
//		viewY = (viewportHeight / zoom / 2);
//
//	}
//}

//void Camera::buildingClampedPosition(Vector2& position) {
//
//
//	Vector2 cameraMax = Vector2(
//		buildingWidth - viewX,
//		buildingHeight - viewY);
//
//	Vector2 cameraMin = Vector2(viewX, viewY);
//	if (cameraMax.x < cameraMin.x)
//		position.Clamp(cameraMax, cameraMin);
//	else
//		position.Clamp(cameraMin, cameraMax);
//}



Matrix Camera::translationMatrix() {
	// casting to int prevents filtering artifacts??
	return Matrix::CreateTranslation(-(int) cameraPosition.x, -(int) cameraPosition.y, 0)
		* Matrix::CreateRotationZ(rotation)
		* Matrix::CreateScale(zoom, zoom, 1)
		* Matrix::CreateTranslation(viewportCenter);
}
