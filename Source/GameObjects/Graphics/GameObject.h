#pragma once
#include "../../../DXTKGui/BaseGraphics/Sprite.h"





class GameObject : public Sprite {
public:
	GameObject();
	virtual ~GameObject();

	virtual void load(GraphicsAsset* const graphicsAsset) override;
	virtual void draw(SpriteBatch* batch) override;
	/** Used for debugging only! */
	virtual void updateDebug(double deltaTime);

	virtual void setPosition(const Vector2& position) override;
	virtual void setRotation(const float rotation) override;
	bool isExploding = false;


	/* Extract int from node text. */
	float getFloatFrom(xml_node node);
	/* Extract int from node attribute. */
	float getFloatFrom(xml_attribute attr);

protected:
	class SubHitArea {
	public:
		SubHitArea(const Vector2& locOffset, const Vector2& size) : locationOffset(locOffset) {
			hitArea.size = size;
		}
		virtual ~SubHitArea() {
		}
		void setPosition(const Vector2& shipPosition) {
			hitArea.position = shipPosition + locationOffset;
		}
		bool collision(_In_ const HitArea& other) const {
			return hitArea.collision(other);
		}

	private:
		Vector2 locationOffset;
		HitArea hitArea;
	};
private:
	float getFloatFrom(string value);


	unique_ptr<RectangleFrame> hbFrame;
};