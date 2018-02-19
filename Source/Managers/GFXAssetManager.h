#pragma once
#include <map>
#include "../../DXTKGui/BaseGraphics/GraphicsAsset.h"

class GFXAssetManager {
public:
	
	~GFXAssetManager();

	bool initialize(ComPtr<ID3D11Device> device, xml_node gfxAssetsNode);

	void reInitDevice(ComPtr<ID3D11Device> device);

	unique_ptr<Sprite> getSpriteFromAsset(const char_t* assetName);
	Animation* getAnimation(const char_t* animationName);
	GraphicsAsset* const getAsset(const char_t* assetName);
	AssetSet* const getAssetSet(const char_t* setName);


private:
	xml_node gfxAssetsNode;

	map<string, unique_ptr<GraphicsAsset> > assetMap;
	map<string, unique_ptr<Animation> > animationMap;
	map<string, unique_ptr<AssetSet> > setMap;


	bool getGFXAssetsFromXML(ComPtr<ID3D11Device> device);



};