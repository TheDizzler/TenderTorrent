#include "../pch.h"
#include "LevelSelectScreen.h"
#include "../assets.h"
#include "../Engine/GameEngine.h"
#include "GameManager.h"



LevelSelectScreen::LevelSelectScreen(MenuManager* menu) {
	menuManager = menu;
}

LevelSelectScreen::~LevelSelectScreen() {
}


bool LevelSelectScreen::initialize(ComPtr<ID3D11Device> device) {

	levelManifest.reset(new xml_document());
	if (!levelManifest->load_file(Assets::levelManifestFile)) {
		wostringstream wss;
		wss << "Unable to load Level Manifest file " << Assets::levelManifestFile;
		GameEngine::showWarningDialog(wss.str(), L"The end is nigh");
		return false;
	}

	titleLabel.reset(guiFactory.createTextLabel(Vector2::Zero, L"Choose a Level"));
	titleLabel->setScale(Vector2(3, 3));
	titleLabel->setTint(Colors::Black);
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

void LevelSelectScreen::reloadGraphicsAssets() {
	titleLabel->reloadGraphicsAsset();
	for (auto const& selection : levelSelections)
		selection->reloadGraphicsAssets();
}

void LevelSelectScreen::setGameManager(GameManager* gm) {
	game = gm;
}


void LevelSelectScreen::update(double deltaTime) {


	if (keys.isKeyPressed(Keyboard::Escape)) {
		menuManager->openMainMenu();
	}

	titleLabel->update(deltaTime);
	for (auto const& selection : levelSelections)
		selection->update(deltaTime);
}

void LevelSelectScreen::draw(SpriteBatch* batch) {
	/*batch->Begin(SpriteSortMode_Deferred);
	{*/
	titleLabel->draw(batch);
	for (auto const& selection : levelSelections) {
		selection->draw(batch);
	}
/*}
batch->End();*/
}

void LevelSelectScreen::pause() {
}

void LevelSelectScreen::controllerRemoved(ControllerSocketNumber controllerSlot, PlayerSlotNumber slotNumber) {
}

void LevelSelectScreen::newController(shared_ptr<Joystick> newStick) {
}

void LevelSelectScreen::loadLevel(string levelXMLFile) {

	menuManager->loadLevel(levelXMLFile);
}




LevelSelection::LevelSelection(const Vector2& position,
	xml_node levelNode) {

	//position = pos;
	xml_node properties = levelNode.parent().child("properties");

	Vector2 frameMargin = Vector2(
		properties.child("frameMargin").attribute("x").as_float(),
		properties.child("frameMargin").attribute("y").as_float());
	Vector2 picpos = position + frameMargin;

	Vector2 picsize = Vector2(
		properties.child("frameDimensions").attribute("x").as_float(),
		properties.child("frameDimensions").attribute("y").as_float());

	xml_node color = properties.child("frameColor");
	frameColor = Vector4(
		color.attribute("r").as_float(),
		color.attribute("g").as_float(),
		color.attribute("b").as_float(),
		color.attribute("a").as_float()
	);

	picFrame.reset(
		guiFactory.createRectangleFrame(picpos, picsize,
			properties.child("frameThickness").attribute("value").as_int(), frameColor));

	Vector2 labelpos = picpos;
	labelpos.y += picsize.y + frameMargin.y;
	label.reset(guiFactory.createTextLabel(labelpos));
	label->setText(levelNode.attribute("name").as_string());
	labelColor = label->getTint();

	levelXMLFile = levelNode.parent().attribute("dir").as_string();
	levelXMLFile += levelNode.attribute("dir").as_string();
	levelXMLFile += levelNode.attribute("file").as_string();



	previewPic.reset(new Sprite(picpos));
	previewPic->load(
		gfxAssets.getAsset(levelNode.attribute("preview").as_string()));
	previewPic->setOrigin(Vector2::Zero);

	Vector2 labelsize = label->measureString();
	Vector2 totalsize = Vector2(picsize.x + frameMargin.x * 2,
		picsize.y + frameMargin.y * 3 + labelsize.y);
	hitArea.reset(new HitArea(position, totalsize));

}

LevelSelection::~LevelSelection() {
}

void LevelSelection::reloadGraphicsAssets() {
	previewPic->reloadGraphicsAsset(&guiFactory);
	picFrame->reloadGraphicsAsset();
	label->reloadGraphicsAsset();
}

void LevelSelection::update(double deltaTime) {

	if (hitArea->contains(mouse.getPosition())) {
		isHover = true;
		if (!isPressed) {
			onHover();
			setToHoverState();
		}
	} else {
		//isPressed = false;
		isHover = false;
	}

	if (isPressed && !mouse.leftButton()) {
		isClicked = true;
		isPressed = false;
		onClick();
		setToUnpressedState();
	} else {
		isClicked = false;
		if (!isHover) {
			isPressed = false;
			setToUnpressedState();
		} else if (mouse.pressed()) {
			isPressed = true;
			setToSelectedState();
		}
	}

	picFrame->update();
	label->update(deltaTime);
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
	picFrame->setTint(frameColor, true);
	label->setTint(labelColor);
}

void LevelSelection::setToHoverState() {

	previewPic->setTint(Vector4(1, 1, 1, .75));
	picFrame->setTint(hoverFrameColor, true);
	label->setTint(hoverLabelColor);
}

void LevelSelection::setToSelectedState() {
}


