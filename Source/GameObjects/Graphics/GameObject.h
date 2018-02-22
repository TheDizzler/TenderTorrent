#pragma once
#include "../../../DXTKGui/BaseGraphics/Sprite.h"





class GameObject : public Sprite {
public:
	GameObject();
	virtual ~GameObject();

	bool isExploding = false;
//protected:

	/* Extract int from node text. */
	float getFloatFrom(xml_node node);
	/* Extract int from node attribute. */
	float getFloatFrom(xml_attribute attr);
private:
	float getFloatFrom(string value);
};