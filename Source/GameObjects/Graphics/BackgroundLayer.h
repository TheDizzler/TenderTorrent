#include "../../pch.h"
#pragma once

#include "../PlayerShip.h"
#include "Tatter.h"

class BackgroundLayer {
public:
	BackgroundLayer();
	~BackgroundLayer();

	void load(GraphicsAsset* const graphicsAsset, shared_ptr<Sprite> cornerFrame);
	void loadPiece(GraphicsAsset* const graphicsAsset);

	void setHealth(int health);
	void setInitialPosition(const Vector2& position, const Vector2& scale);
	void takeDamage(int damageTaken);
	
	/* Image gets layerDepth, frame gets layerDepth + .1 */
	void setLayerDepth(float layerDepth);
	void moveBy(const Vector2& position);

	void update(double deltaTime);
	void draw(SpriteBatch* batch);

	const HitArea* getHitArea() const;

	/* example of use:
	control->setMatrixFunction([&]() -> Matrix { return camera->translationMatrix(); }); */
	void setMatrixFunction(function<Matrix()> translationMat) {
		translationMatrix = translationMat;
	}
	void setCameraZoom(function<float()> zoomFunction) {
		cameraZoom = zoomFunction;
	}

	virtual void updateProjectedHitArea();

	virtual const Vector2& getScreenPosition(Matrix viewProjectionMatrix) const;
	virtual unique_ptr<HitArea> getScreenHitArea(Matrix viewProjectionMatrix) const;


	bool isAlive();
private:
	function<Matrix()> translationMatrix;
	function<float()> cameraZoom;
	unique_ptr<HitArea> projectedHitArea;

	ComPtr<ID3D11ShaderResourceView> frameTexture;
	RECT frameRect;
	Color frameTint;
	Vector2 frameOrigin;
	float frameLayerDepth = .5;

	Vector2 topLeftCornerPos, topRightCornerPos,
		bottomLeftCornerPos, bottomRightCornerPos;

	int health = 200;

	unique_ptr<TextLabel> healthLabel;

	bool labelHidden = true;

	unique_ptr<Sprite> whole;
	vector<unique_ptr<Tatter> > tatters;

	Vector2 scale = Vector2(1, 1);

	double timeExploding = 0;
	const float EXPLODE_TIME = 1.5;
	
};