#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>


#include "../globals.h"
#include "../Managers/LevelManager.h"


/** Based on
https://roguesharp.wordpress.com/2014/07/13/tutorial-5-creating-a-2d-camera-with-pan-and-zoom-in-monogame/ */
class Camera {
public:
	Camera();
	virtual ~Camera();

	void setLevel(Background* bgMan);

	void setViewport(D3D11_VIEWPORT cameraViewport);
	void setViewport(int viewportWidth, int viewportHeight);
	void updateViewport(const Vector2& viewportArea,
		const Vector2& viewportPosition, bool zoomToFit = false);

	float rotation = 0.0f;

	Vector2 viewportPosition = Vector2::Zero;
	int viewportWidth;
	int viewportHeight;
	Vector3 viewportCenter;

	/* Prevents GameObject from leaving viewable screen. */
	void confineToScreen(GameObject* obj);
	/* Returns true if point (in WORLD coords) is on screen. */
	bool viewContains(const Vector2& point);

	/* Get world coordinates of camera. */
	const Vector2& getPosition() const;
	/* Amount screen has moved since last update*/
	const Vector2& getDelta() const;
	float getZoom();
	void setZoomToResolution(int width = Globals::targetResolution.x,
		int height = Globals::targetResolution.y);
	void setZoom(float zoomAmount);
	void adjustZoom(float amount);
	
	void moveCamera(const Vector2& cameraMovement);

	//RECT* viewportWorldBoundary();

	void centerOn(const Vector2& pos);

	Matrix translationMatrix();
	Vector2 worldToScreen(Vector2 worldPosition);
	Vector2 screenToWorld(Vector2 screenPosition);

private:

	Vector2 position;
	Vector2 positionDelta;

	float zoom;
	float levelWidth;
	float levelHeight;

};