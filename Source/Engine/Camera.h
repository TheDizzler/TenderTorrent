#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>


#include "../globals.h"
#include "../Managers/LevelManager.h"


/** Based on
https://roguesharp.wordpress.com/2014/07/13/tutorial-5-creating-a-2d-camera-with-pan-and-zoom-in-monogame/ */
class Camera {
public:
	Camera(int viewportWidth, int viewportHeight);
	virtual ~Camera();

	void setLevel(Background* bgMan);

	void updateViewport(const Vector2& viewportArea, const Vector2& viewportPosition,
		bool zoomToFit = false);


	//Viewport* viewport;


	float rotation = 0.0f;

	Vector2 viewportPosition = Vector2::Zero;
	int viewportWidth;
	int viewportHeight;
	Vector3 viewportCenter;

	bool viewContains(const Vector2& point);

	float getZoom();
	void setZoomToResolution(int width = Globals::targetResolution.x,
		int height = Globals::targetResolution.y);
	void setZoom(float zoomAmount);
	void adjustZoom(float amount);
	
	void moveCamera(const Vector2& cameraMovement);
	void setCameraPosition(const Vector2& newPosition);

	RECT* viewportWorldBoundary();

	void centerOn(const Vector2& pos);

	Matrix translationMatrix();
	Vector2& worldToScreen(Vector2 worldPosition);
	Vector2& screenToWorld(Vector2 screenPosition);
private:

	Vector2 cameraPosition;

	float zoom;
	float levelWidth;
	float levelHeight;

	//float viewX = (viewportWidth / zoom / 2);
	//float viewY = (viewportHeight / zoom / 2);


	//void zoomToFitBackground();

};