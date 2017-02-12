#include "../../pch.h"
#pragma once

#include "../PlayerShip.h"
#include "../../DXTKGui/Controls/TextLabel.h"


class BackgroundLayer /*: public Sprite */ {
public:
	BackgroundLayer();
	~BackgroundLayer();

	void load(GraphicsAsset* const graphicsAsset, shared_ptr<Sprite> cornerFrame);
	void loadPiece(GraphicsAsset* const graphicsAsset);
	//void setHitArea(const Vector2& position, const Vector2& size);

	void setHealth(int health);
	void setInitialPosition(const Vector2& position, const Vector2& scale);
	void takeDamage(int damageTaken);
	
	/* Image gets layerDepth, frame gets layerDepth + .1 */
	void setLayerDepth(float layerDepth);
	void moveBy(const Vector2& position);

	void update(double deltaTime/*, shared_ptr<MouseController> mouse*/);
	void draw(SpriteBatch* batch/*, Sprite* frame*/);

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
	vector<unique_ptr<Sprite> > tatters;
	//shared_ptr<Sprite> frame;

	Vector2 scale = Vector2(1, 1);
	
};