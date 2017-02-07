#include "../pch.h"
#pragma once


class AssetSet {
public:
	AssetSet(const char_t* setName);

	void addAsset(string assetName, unique_ptr<GraphicsAsset> asset);
	GraphicsAsset* const getAsset(const char_t* assetName);
private:

	const char_t* setName;
	map<string, unique_ptr<GraphicsAsset> > assetMap;
};



class GFXAssetManager {
public:
	GFXAssetManager(xml_node gfxAssetsNode);
	~GFXAssetManager();

	bool initialize(ComPtr<ID3D11Device> device);

	unique_ptr<Sprite> getSpriteFromAsset(const char_t* assetName);
	shared_ptr<Animation> getAnimation(const char_t* animationName);
	GraphicsAsset* const getAsset(const char_t* assetName);
	shared_ptr<AssetSet> const getAssetSet(const char_t* setName);


private:
	xml_node gfxAssetsNode;

	map<string, unique_ptr<GraphicsAsset> > assetMap;
	map<string, shared_ptr<Animation> > animationMap;
	map<string, shared_ptr<AssetSet> > setMap;


	bool getGFXAssetsFromXML(ComPtr<ID3D11Device> device);



};