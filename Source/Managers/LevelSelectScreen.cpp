#include "LevelSelectScreen.h"

using namespace pugi;

LevelSelectScreen::LevelSelectScreen() {
}

LevelSelectScreen::~LevelSelectScreen() {
}

#include "../assets.h"
#include "../Engine/GameEngine.h"
bool LevelSelectScreen::initialize(ComPtr<ID3D11Device> device, MouseController* mouse) {

	levelManifest.reset(new xml_document());
	if (!levelManifest->load_file(Assets::levelManifestFile)) {
		wostringstream wss;
		wss << "Unable to load Level Manifest file " << Assets::levelManifestFile;
		GameEngine::showWarningDialog(wss.str(), L"The end is nigh");
		return false;
	}

	

	return true;
}

void LevelSelectScreen::setGameManager(GameManager* gm) {
	game = gm;
}

void LevelSelectScreen::update(double deltaTime,
	KeyboardController* keys, MouseController* mouse) {


}

void LevelSelectScreen::draw(SpriteBatch* batch) {
}

void LevelSelectScreen::pause() {
}



#include "GameManager.h"
LevelSelection::LevelSelection(const Vector2& position,
	xml_node levelNode) {

	xml_node properties = levelNode.parent().child("properties");

	Vector2 frameMargin = Vector2(properties.child("frameMargin").attribute("x").as_int(),
		properties.child("frameMargin").attribute("y").as_int());
	Vector2 picpos = position + frameMargin;

	Vector2 picsize = Vector2(properties.child("frameDimensions").attribute("x").as_int(),
		properties.child("frameDimensions").attribute("y").as_int());

	xml_node color = properties.child("frameColor");
	Vector4 framecolor{
		color.attribute("r").as_float(),
		color.attribute("g").as_float(),
		color.attribute("b").as_float(),
		color.attribute("a").as_float()
	};



	picFrame.reset(
		GameManager::guiFactory->createRectangleFrame(picpos, picsize, 
		properties.child("frameThickness").attribute("value").as_int(), framecolor));

	Vector2 labelpos = picpos;
	labelpos.y += frameMargin.y;
	label.reset(GameManager::guiFactory->createTextLabel(labelpos, levelNode.attribute("name").as_string()));

	levelXMLFile = levelNode.parent().attribute("dir").as_string();
	levelXMLFile += levelNode.attribute("dir").as_string();
	levelXMLFile += levelNode.attribute("file").as_string();


	previewPic.reset(new Sprite(picpos));
	previewPic->load(GameManager::gfxAssets->getAsset(levelNode.attribute("preview").as_string()));
}
