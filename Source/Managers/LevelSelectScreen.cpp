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

	selectorManager.initialize(make_unique<GrowSelector>(&guiFactory));
	selectorManager.setControllers(joystick, &keys, &mouse);

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

		LevelSelection* level = new LevelSelection(&guiFactory, &mouse, pos, levelNode);
		pos.x += level->getSize().x + 10;
		level->setOnClickListener(new OnClickLevelSelect(this));
		//levelSelections.push_back(unique_ptr<LevelSelection>(level));
		selectorManager.addControl(level);
	}


	return true;
}

void LevelSelectScreen::reloadGraphicsAssets() {
	selectorManager.reloadGraphicsAssets();
	titleLabel->reloadGraphicsAsset();
}

void LevelSelectScreen::setGameManager(GameManager* gm) {
	game = gm;
}


void LevelSelectScreen::update(double deltaTime) {

	
	if (keys.isKeyPressed(Keyboard::Escape)
	|| joystick->selectButtonPushed()
	|| joystick->bButtonPushed()) {
		menuManager->openMainMenu();
	}

	titleLabel->update(deltaTime);
	selectorManager.update(deltaTime);
}

void LevelSelectScreen::draw(SpriteBatch* batch) {

	titleLabel->draw(batch);
	selectorManager.draw(batch);
}

void LevelSelectScreen::pause() {
}

void LevelSelectScreen::controllerRemoved(ControllerSocketNumber controllerSlot,
	PlayerSlotNumber slotNumber) {
}

void LevelSelectScreen::newController(shared_ptr<Joystick> newStick) {
	selectorManager.setJoystick(newStick.get());
}

void LevelSelectScreen::loadLevel(string levelXMLFile) {

	menuManager->loadLevel(levelXMLFile);
}




LevelSelection::LevelSelection(GUIFactory* factory, MouseController* mouseController,
	const Vector2& position, xml_node levelNode)
	: Selectable(factory, mouseController) {

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



	previewPic.setPosition(picpos);
	previewPic.load(
		gfxAssets.getAsset(levelNode.attribute("preview").as_string()));
	previewPic.setOrigin(Vector2::Zero);
	previewPic.setTint(Vector4(1, 1, 1, .75));

	Vector2 labelsize = label->measureString();
	Vector2 totalsize = Vector2(picsize.x + frameMargin.x * 2,
		picsize.y + frameMargin.y * 3 + labelsize.y);
	hitArea.position = position;
	hitArea.size = totalsize;
}

LevelSelection::~LevelSelection() {
	if (onClickListener != NULL)
		delete onClickListener;
}

void LevelSelection::reloadGraphicsAsset() {
	previewPic.reloadGraphicsAsset(guiFactory);
	picFrame->reloadGraphicsAsset();
	label->reloadGraphicsAsset();
}



bool LevelSelection::updateSelect(double deltaTime) {

	bool refreshTexture = false;

	if (picFrame->update())
		refreshTexture = true;
	if (label->update(deltaTime))
		refreshTexture = true;

	return refreshTexture;
}

bool LevelSelection::update(double deltaTime) {

	bool refreshTexture = false;

	if (hitArea.contains(mouse->getPosition())) {
		isHover = true;
		if (!isPressed) {
			onHover();
			setToHoverState();
		}
	} else {
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

	picFrame->update();
	label->update(deltaTime);

	return false;
}

void LevelSelection::draw(SpriteBatch* batch) {

	previewPic.draw(batch);
	picFrame->draw(batch);
	label->draw(batch);
}

const Vector2& LevelSelection::getSize() {
	return hitArea.size;
}

bool LevelSelection::clicked() {
	if (isClicked) {
		isClicked = isPressed = false;
		return true;
	}

	return false;
}

bool LevelSelection::pressed() {
	return isPressed;
}

bool LevelSelection::hovering() {
	return isHover;
}

void LevelSelection::setOnClickListener(OnClickListener* iOnC) {
	if (onClickListener != NULL)
		delete onClickListener;
	onClickFunction = &OnClickListener::onClick;
	onPressFunction = &OnClickListener::onPress;
	onHoverFunction = &OnClickListener::onHover;
	onResetFunction = &OnClickListener::resetState;
	onClickListener = iOnC;
}


void LevelSelection::onClick() {

	isClicked = true;
	if (onClickListener != NULL) {
		isClicked = isPressed = false;
		(onClickListener->*onClickFunction)(this);
	}
	resetState();
	hasBeenSetUnpressed = false;
}


void LevelSelection::onPress() {

	isPressed = true;
	if (onClickListener != NULL) {
		(onClickListener->*onPressFunction)(this);
	}
	setToSelectedState();
	hasBeenSetUnpressed = false;
	hasBeenSetHover = false;
}


void LevelSelection::onHover() {

	isHover = true;
	if (onClickListener != NULL) {
		(onClickListener->*onHoverFunction)(this);
	}
	setToHoverState();
	hasBeenSetHover = true;
	hasBeenSetUnpressed = false;
}

void LevelSelection::resetState() {
	if (onClickListener != NULL) {
		(onClickListener->*onResetFunction)(this);
	}
	isHover = false;
	mouseHover = false;
	isPressed = false;
	setToUnpressedState();
	hasBeenSetHover = false;
}


void LevelSelection::setToUnpressedState() {

	previewPic.setTint(Vector4(1, 1, 1, .75));
	picFrame->setTint(frameColor, true);
	label->setTint(labelColor);
}

void LevelSelection::setToHoverState() {

	previewPic.setTint(Vector4(1, 1, 1, 1));
	picFrame->setTint(hoverFrameColor, true);
	label->setTint(hoverLabelColor);
}

void LevelSelection::setToSelectedState() {
}

void LevelSelection::setFont(const pugi::char_t* font) {
	label->setFont(font);
}

void LevelSelection::setText(wstring text) {
	label->setText(text);
}

const Vector2 XM_CALLCONV LevelSelection::measureString() const {
	return label->measureString();
}

const Vector2& LevelSelection::getPosition() const {
	return hitArea.position;
}

const int LevelSelection::getWidth() const {
	return hitArea.size.x;
}

const int LevelSelection::getHeight() const {
	return hitArea.size.y;
}




