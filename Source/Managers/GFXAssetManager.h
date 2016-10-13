#pragma once

#include <pugixml.hpp>

#include "../DXTKGui/BaseGraphics/Sprite.h"

using namespace pugi;


class GFXAssetManager {
public:
	GFXAssetManager(xml_node gfxAssetsNode);
	~GFXAssetManager();
	
	bool initialize(ComPtr<ID3D11Device> device);

	unique_ptr<Sprite> getSpriteFromAsset(const char_t* assetName);
	shared_ptr<Animation> getAnimation(const char_t* animationName);
	GraphicsAsset* const getAsset(const char_t* assetName);

private:
	xml_node gfxAssetsNode;


	bool getGFXAssetsFromXML();

};