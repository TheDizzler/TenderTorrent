#include "../pch.h"
#pragma once

#include <map>

#include "../DXTKGui/BaseGraphics/Sprite.h"


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

	map<string, unique_ptr<GraphicsAsset> > assetMap;
	map<string, shared_ptr<Animation> > animationMap;

	bool getGFXAssetsFromXML(ComPtr<ID3D11Device> device);

};