#include "GFXAssetManager.h"

GFXAssetManager::GFXAssetManager(xml_node gfxAN) {
	gfxAssetsNode = gfxAN;
}

GFXAssetManager::~GFXAssetManager() {
	assetMap.clear();
}

bool GFXAssetManager::initialize(ComPtr<ID3D11Device> device) {

	if (!getGFXAssetsFromXML(device)) {
		MessageBox(0, L"Sprite retrieval from Asset Manifest failed.",
			L"Epic failure", MB_OK);
		return false;
	}
	return true;
}

#include <sstream>
unique_ptr<Sprite> GFXAssetManager::getSpriteFromAsset(const char_t* assetName) {

	GraphicsAsset* const asset = getAsset(assetName);
	if (asset == NULL)
		return NULL;
	unique_ptr<Sprite> sprite;
	sprite.reset(new Sprite());
	sprite->load(asset);
	return sprite;
}

shared_ptr<Animation> GFXAssetManager::getAnimation(const char_t * animationName) {
	
	if (animationMap.find(animationName) == animationMap.end()) {
		wostringstream ws;
		ws << "Cannot find asset file: " << animationName << "\n";
		ws << "Count : " << animationMap.count(animationName) << "\n";
		OutputDebugString(ws.str().c_str());
		return NULL;
	}

	return animationMap[animationName];

}

GraphicsAsset* const GFXAssetManager::getAsset(const char_t * assetName) {

	if (assetMap.find(assetName) == assetMap.end()) {
		wostringstream ws;
		ws << "Cannot find asset file: " << assetName << "\n";
		OutputDebugString(ws.str().c_str());
		return NULL;
	}

	return assetMap[assetName].get();
}

#include "../DXTKGui/StringHelper.h"
bool GFXAssetManager::getGFXAssetsFromXML(ComPtr<ID3D11Device> device) {

	string assetsDir =
		gfxAssetsNode.parent().attribute("dir").as_string();

	string gfxDir = assetsDir + gfxAssetsNode.attribute("dir").as_string();


	for (xml_node spriteNode = gfxAssetsNode.child("sprite"); spriteNode;
		spriteNode = spriteNode.next_sibling("sprite")) {

		string file_s = gfxDir + spriteNode.attribute("file").as_string();
		const char_t* file = file_s.c_str();
		const char_t* name = spriteNode.attribute("name").as_string();
		string check = name;

		unique_ptr<GraphicsAsset> guiAsset;
		guiAsset.reset(new GraphicsAsset());
		if (!guiAsset->load(device, StringHelper::convertCharStarToWCharT(file))) {
			wstringstream wss;
			wss << "Unable to load file: " << file;
			MessageBox(0, wss.str().c_str(), L"Critical error", MB_OK);
			return false;
		}

		assetMap[check] = move(guiAsset);
	}



	return true;
}
