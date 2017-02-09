#include "LevelSelectScreen.h"

using namespace pugi;

#include "MenuManager.h"
LevelSelectScreen::LevelSelectScreen(MenuManager* menu) {
	menuManager = menu;
}

LevelSelectScreen::~LevelSelectScreen() {
}

#include "../assets.h"
#include "../Engine/GameEngine.h"
#include "GameManager.h"
bool LevelSelectScreen::initialize(ComPtr<ID3D11Device> device, shared_ptr<MouseController> mouse) {


	levelManifest.reset(new xml_document());
	if (!levelManifest->load_file(Assets::levelManifestFile)) {
		wostringstream wss;
		wss << "Unable to load Level Manifest file " << Assets::levelManifestFile;
		GameEngine::showWarningDialog(wss.str(), L"The end is nigh");
		return false;
	}

	titleLabel.reset(guiFactory->createTextLabel(Vector2::Zero, L"Choose a Level"));
	titleLabel->setScale(Vector2(3, 3));
	Vector2 titlesize = titleLabel->measureString();
	Vector2 titlepos = Vector2((Globals::WINDOW_WIDTH - titlesize.x) / 2, 10);
	titleLabel->setPosition(titlepos);

	Vector2 pos = titlepos;
	pos.y += titlesize.y + 10;
	for (xml_node levelNode = levelManifest->child("root").child("level");
		levelNode; levelNode = levelNode.next_sibling("level")) {

		LevelSelection* level = new LevelSelection(pos, levelNode);
		pos.x += level->getSize().x + 10;
		level->setOnClickListener(new OnClickLevelSelect(this));
		levelSelections.push_back(unique_ptr<LevelSelection>(level));
	}




	return true;
}

void LevelSelectScreen::setGameManager(GameManager* gm) {
	game = gm;
}

//Keyboard::KeyboardStateTracker keyTracker;
void LevelSelectScreen::update(double deltaTime, shared_ptr<MouseController> mouse) {

	auto state = Keyboard::Get().GetState();
	//keyTracker.Update(state);

	if (state.Escape) {
		menuManager->openMainMenu();
	}

	for (auto const& selection : levelSelections)
		selection->update(deltaTime, mouse);
}

void LevelSelectScreen::draw(SpriteBatch* batch) {
	batch->Begin(SpriteSortMode_Deferred);
	{
		titleLabel->draw(batch);
		for (auto const& selection : levelSelections) {
			selection->draw(batch);
		}
	}
	batch->End();
}

void LevelSelectScreen::safedraw(SpriteBatch* batch) {
	titleLabel->draw(batch);
	for (auto const& selection : levelSelections) {
		selection->draw(batch);
	}
}

void LevelSelectScreen::pause() {
}

void LevelSelectScreen::loadLevel(string levelXMLFile) {

	menuManager->loadLevel(levelXMLFile);
}




LevelSelection::LevelSelection(const Vector2& position,
	xml_node levelNode) {

	//position = pos;
	xml_node properties = levelNode.parent().child("properties");

	Vector2 frameMargin = Vector2(
		properties.child("frameMargin").attribute("x").as_int(),
		properties.child("frameMargin").attribute("y").as_int());
	Vector2 picpos = position + frameMargin;

	Vector2 picsize = Vector2(
		properties.child("frameDimensions").attribute("x").as_int(),
		properties.child("frameDimensions").attribute("y").as_int());

	xml_node color = properties.child("frameColor");
	frameColor = Vector4(
		color.attribute("r").as_float(),
		color.attribute("g").as_float(),
		color.attribute("b").as_float(),
		color.attribute("a").as_float()
	);

	picFrame.reset(
		guiFactory->createRectangleFrame(picpos, picsize,
			properties.child("frameThickness").attribute("value").as_int(), frameColor));

	Vector2 labelpos = picpos;
	labelpos.y += picsize.y + frameMargin.y;
	label.reset(guiFactory->createTextLabel(labelpos));
	label->setText(levelNode.attribute("name").as_string());
	labelColor = label->getTint();

	levelXMLFile = levelNode.parent().attribute("dir").as_string();
	levelXMLFile += levelNode.attribute("dir").as_string();
	levelXMLFile += levelNode.attribute("file").as_string();



	previewPic.reset(new Sprite(picpos));
	previewPic->load(
		gfxAssets->getAsset(levelNode.attribute("preview").as_string()));
	previewPic->setOrigin(Vector2::Zero);

	Vector2 labelsize = label->measureString();
	Vector2 totalsize = Vector2(picsize.x + frameMargin.x * 2,
		picsize.y + frameMargin.y * 3 + labelsize.y);
	hitArea.reset(new HitArea(position, totalsize));

}

void LevelSelection::update(double deltaTime, shared_ptr<MouseController> mouse) {

	if (hitArea->contains(mouse->getPosition())) {
		isHover = true;
		if (!isPressed) {
			onHover();
			setToHoverState();
		}
	} else {
		//isPressed = false;
		isHover = false;
	}

	if (isPressed && !mouse->leftButton()) {
		isClicked = true;
		isPressed = false;
		onClick();
		setToUnpressedState();
	} else {
		isClicked = false;
		if (!isHover) {
			isPressed = false;
			setToUnpressedState();
		} else if (mouse->pressed()) {
			isPressed = true;
			setToSelectedState();
		}
	}
}

void LevelSelection::draw(SpriteBatch* batch) {

	previewPic->draw(batch);
	picFrame->draw(batch);
	label->draw(batch);
}

const Vector2& LevelSelection::getSize() {
	return hitArea->size;
}



void LevelSelection::setToUnpressedState() {

	previewPic->setTint(Vector4(1, 1, 1, 1));
	picFrame->setTint(frameColor);
	label->setTint(labelColor);
}

void LevelSelection::setToHoverState() {

	previewPic->setTint(Vector4(1, 1, 1, .75));
	picFrame->setTint(hoverFrameColor);
	label->setTint(hoverLabelColor);
}

void LevelSelection::setToSelectedState() {
}


