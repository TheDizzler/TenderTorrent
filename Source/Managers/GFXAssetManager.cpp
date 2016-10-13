#include "GFXAssetManager.h"

GFXAssetManager::GFXAssetManager(xml_node gfxAN) {
	gfxAssetsNode = gfxAN;
}

bool GFXAssetManager::initialize(ComPtr<ID3D11Device> device) {

	if (!getGFXAssetsFromXML()) {
		MessageBox(0, L"Sprite retrieval from Asset Manifest failed.",
			L"Epic failure", MB_OK);
		return false;
	}
	return true;
}

bool GFXAssetManager::getGFXAssetsFromXML() {
	return false;
}
