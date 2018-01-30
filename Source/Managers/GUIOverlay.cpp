#include "../pch.h"
#include "GUIOverlay.h"
#include "GameManager.h"

GUIOverlay::GUIOverlay() {

	hudBG.reset(guiFactory->createRectangle(Vector2::Zero, Vector2(Globals::WINDOW_WIDTH, 55)));
	hudBG->setTint(Vector4(1, 0, 1, 1));

	int borderThickness = 4;
	Vector2 borderpos = hudBG->getPosition();
	borderpos.y -= borderThickness;
	Vector2 bordersize = hudBG->getSize();
	bordersize.y += borderThickness;
	hudBorder.reset(guiFactory->createRectangleFrame(borderpos, bordersize, borderThickness));


	timerLabel.reset(
		guiFactory->createTextLabel(Vector2(500, 3), L"", "Default Font", false));
	timerLabel->setTint(Vector4(0, 0, 0, 1));


	scoreLabel.reset(guiFactory->createTextLabel(Vector2(10, 3)));
	scoreLabel->setTint(Vector4(0, 0, 0, 1));


	energyLabel.reset(guiFactory->createTextLabel(Vector2(10, 25)));
	energyLabel->setTint(Vector4(0, 0, 0, 1));


	pauseOverlay.reset(
		guiFactory->createRectangle(Vector2(0, 0),
			Vector2(Globals::WINDOW_WIDTH, Globals::WINDOW_HEIGHT)));
	pauseOverlay->setTint(Color(1, .588, 1, .8)); //should be pinkish


	pauseLabel.reset(
		guiFactory->createTextLabel(Vector2::Zero, L"Blank", "BlackCloak", false));
	pauseLabel->setText(L"Paused");
	pauseLabel->setScale(Vector2(1, 1.5));
	Vector2 size = pauseLabel->measureString();
	pauseLabel->setPosition(Vector2(
		(Globals::WINDOW_WIDTH - size.x) / 2, (Globals::WINDOW_HEIGHT - size.y) / 2));

	gameOverLabel.reset(
		guiFactory->createTextLabel(Vector2::Zero, L"Game Over", "BlackCloak", false));
	gameOverLabel->setScale(Vector2(1, 1.5));
	size = gameOverLabel->measureString();
	gameOverLabel->setPosition(Vector2(
		(Globals::WINDOW_WIDTH - size.x) / 2, (Globals::WINDOW_HEIGHT - size.y) / 2));
	gameOverLabel->setTint(Color(0, 0, 0, 1));

	exitButton.reset(guiFactory->createImageButton(
		Vector2(Globals::WINDOW_WIDTH / 4, Globals::WINDOW_HEIGHT * 3 / 4),
		"Button Up", "Button Down"));
	exitButton->setText(L"Exit");
	Vector2 moveBy = Vector2(exitButton->getScaledWidth() / 2, 0);
	exitButton->moveBy(-moveBy);



	continueButton.reset(guiFactory->createImageButton(
		Vector2(Globals::WINDOW_WIDTH * 3 / 4, Globals::WINDOW_HEIGHT * 3 / 4),
		"Button Up", "Button Down"));
	continueButton->setText(L"Continue");
	continueButton->moveBy(-moveBy);




	warningLabel.reset(guiFactory->createTextLabel(
		Vector2((Globals::WINDOW_WIDTH) / 2, (Globals::WINDOW_HEIGHT) / 2),
		L"GET READY!", "BlackCloak", false));
	warningLabel->setScale(Vector2(1, 1.5));
	size = warningLabel->measureString(L"GET READY!");
	warningLabel->moveBy(-size / 2);

	fpsLabel.reset(guiFactory->createTextLabel(Vector2(10, Globals::WINDOW_HEIGHT - 50),
		L"FPS"));
}

GUIOverlay::~GUIOverlay() {

}

void GUIOverlay::reloadGraphicsAssets() {

	hudBG->reloadGraphicsAsset(guiFactory.get());
	hudBorder->reloadGraphicsAsset();
	timerLabel->reloadGraphicsAsset();
	scoreLabel->reloadGraphicsAsset();
	energyLabel->reloadGraphicsAsset();
}

const Vector2& GUIOverlay::getPlayArea() {
	return Vector2(Globals::WINDOW_WIDTH, Globals::WINDOW_HEIGHT - hudBG->getHeight());
}

const Vector2& GUIOverlay::getPlayPosition() {
	Vector2 pos = hudBG->getPosition();
	pos.y += hudBG->getHeight();
	return pos;
}

int frameCount = 0;
double fpsUpdateTime = 5;
const double FPS_UPDATE_TIME = 1;
void GUIOverlay::update(double deltaTime) {
	hudBorder->update();
	timerLabel->update(deltaTime);
	scoreLabel->update(deltaTime);
	energyLabel->update(deltaTime);

	fpsUpdateTime += deltaTime;
	++frameCount;
	if (fpsUpdateTime >= FPS_UPDATE_TIME) {

		wostringstream wss;
		wss << "frameCount: " << frameCount << " fpsUpdateTime: " << fpsUpdateTime << endl;
		wss << "fps: " << frameCount / fpsUpdateTime;
		fpsLabel->setText(wss.str());
		fpsLabel->update(deltaTime);
		fpsUpdateTime = 0;
		frameCount = 0;
	}
}


bool rInc = true;
bool gInc = false;
bool bInc = true;
void GUIOverlay::updatePaused(double deltaTime) {

	//displayPause(deltaTime);

	Color color = pauseLabel->getTint();
	/** Changes the Red variable between 0 and 1. */
	if (rInc) {
		color.R(color.R() + deltaTime);
		if (color.R() >= 1)
			rInc = false;
	} else {
		color.R(color.R() - deltaTime);
		if (color.R() <= 0)
			rInc = true;
	}

	pauseLabel->setTint(color);
	pauseLabel->update(deltaTime);
	exitButton->update(deltaTime);
	continueButton->update(deltaTime);

}

void GUIOverlay::updateWarning(double deltaTime) {
	//displayWarning(deltaTime);

	Color color = warningLabel->getTint();
	/** Changes the Green and Blue variables between 0 and 1. */
	float r = color.G();
	if (rInc) {
		r += 5 * deltaTime;
		if (r >= 1)
			rInc = false;
	} else {
		r -= 5 * deltaTime;
		if (r <= 0)
			rInc = true;
	}

	warningLabel->setTint(Vector4(1, r, r, 1));
}

void GUIOverlay::updateGameOver(double deltaTime) {
	gameOverLabel->update(deltaTime);
}


void GUIOverlay::draw(SpriteBatch* batch) {

	hudBG->draw(batch);
	hudBorder->draw(batch);
	timerLabel->draw(batch);
	scoreLabel->draw(batch);
	energyLabel->draw(batch);
	fpsLabel->draw(batch);
}

void GUIOverlay::drawPaused(SpriteBatch* batch) {

	pauseOverlay->draw(batch);
	exitButton->draw(batch);
	continueButton->draw(batch);
	pauseLabel->draw(batch);
	fpsLabel->draw(batch);
}

void GUIOverlay::drawWarning(SpriteBatch* batch) {
	warningLabel->draw(batch);
}

void GUIOverlay::drawGameOver(SpriteBatch* batch) {
	gameOverLabel->draw(batch);
}


/** Changes the Green and Blue variables between 0 and 1. */
//void GUIOverlay::displayWarning(double deltaTime) {
//
//	Color color = warningLabel->getTint();
//	float r = color.G();
//	if (rInc) {
//		r += 5 * deltaTime;
//		if (r >= 1)
//			rInc = false;
//	} else {
//		r -= 5 * deltaTime;
//		if (r <= 0)
//			rInc = true;
//	}
//
//	warningLabel->setTint(Vector4(1, r, r, 1));
//}

/** Changes the Red variable between 0 and 1. */
//void GUIOverlay::displayPause(double deltaTime) {
//
//	Color color = pauseLabel->getTint();
//
//	if (rInc) {
//		color.R(color.R() + deltaTime);
//		if (color.R() >= 1)
//			rInc = false;
//	} else {
//		color.R(color.R() - deltaTime);
//		if (color.R() <= 0)
//			rInc = true;
//	}
//
//	pauseLabel->setTint(color);
//}