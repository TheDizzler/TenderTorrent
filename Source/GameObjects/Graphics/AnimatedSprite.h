#pragma once

#include "../../../DXTKGui/BaseGraphics/GraphicsAsset.h"

class AnimatedSprite : public IElement2D {
public:
	AnimatedSprite();
	AnimatedSprite(const Vector2& position);
	virtual ~AnimatedSprite();

	void load(Animation* animation);

	virtual void update(double deltaTime);
	virtual void draw(SpriteBatch * batch) override;

	virtual const Vector2& getPosition() const override;
	virtual const Vector2& getOrigin() const override;
	virtual const Vector2& getScale() const override;
	virtual const float getRotation() const override;
	virtual const Color& getTint() const override;
	virtual const float getAlpha() const override;
	virtual const int getWidth() const override;
	virtual const int getHeight() const override;
	virtual const float getLayerDepth() const override;
	const RECT getRect() const;
	const HitArea* getHitArea() const;
	
	virtual bool contains(const Vector2 & point) override;
	

	virtual void moveBy(const Vector2& moveVector) override;
	virtual void setPosition(const Vector2& position) override;
	virtual void setOrigin(const Vector2& origin) override;
	virtual void setScale(const Vector2& scale) override;
	virtual void setRotation(const float rotation) override;
	virtual void setTint(const XMFLOAT4 color) override;
	virtual void setTint(const Color& color) override;
	virtual void setTint(const XMVECTORF32 color) override;
	virtual void setAlpha(const float alpha) override;
	virtual void setLayerDepth(const float depth, bool frontToBack = true) override;

	void reset();
	bool repeats = true;
	bool isAlive = true;
	
protected:
	Animation* animation;
	int currentFrameIndex = -1;

	double currentFrameTime = 0;

	Vector2 origin;
	Color tint = DirectX::Colors::White.v;
	float rotation = 0.0f;
	float layerDepth = 0.0f;

	Vector2 position;

	Vector2 scale = Vector2(1, 1);
	HitArea hitArea;
};