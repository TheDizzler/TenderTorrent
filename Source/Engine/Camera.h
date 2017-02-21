#include "../pch.h"
#pragma once

#include "../Managers/LevelManager.h"


/** Based on
https://roguesharp.wordpress.com/2014/07/13/tutorial-5-creating-a-2d-camera-with-pan-and-zoom-in-monogame/ */
class Camera {
public:
	Camera(int viewportWidth, int viewportHeight);
	Camera(const Vector2& viewport);
	~Camera();

	void setLevel(Background* bgMan);

	void updateViewport(const Vector2& viewportArea, const Vector2& viewportPosition);


	Viewport* viewport;
	

	float rotation = 0.0f;

	Vector2 viewportPosition;
	int viewportWidth;
	int viewportHeight;
	Vector3 viewportCenter;


	void adjustZoom(float amount);
	void setZoom(float zoomAmount);
	void moveCamera(const Vector2& cameraMovement);
	void setCameraPosition(const Vector2& newPosition);

	RECT* viewportWorldBoundary();

	void centerOn(const Vector2& pos);

	Matrix translationMatrix();
	Vector2& worldToScreen(Vector2 worldPosition);

	float getZoom();
private:

	Vector2 position;

	float zoom;
	float levelWidth;
	float levelHeight;

	float viewX = (viewportWidth / zoom / 2);
	float viewY = (viewportHeight / zoom / 2);


	void zoomToFitBackground();
	Vector2* screenToWorld(Vector2 screenPosition);
	
};