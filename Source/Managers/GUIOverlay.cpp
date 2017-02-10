#include "GUIOverlay.h"

#include "GameManager.h"

GUIOverlay::GUIOverlay() {

	hudBG.reset(guiFactory->createRectangle(Vector2::Zero, Vector2(Globals::WINDOW_WIDTH, 50)));
	hudBG->setTint(Vector4(1, 0, 1, 1));

	timerLabel.reset(guiFactory->createTextLabel(Vector2(500, 10)));
	timerLabel->setTint(Vector4(0, 0, 0, 1));
	//textLabels.push_back(timerLabel.get());

	//activeControls.push_back(timerLabel.get());

	scoreLabel.reset(guiFactory->createTextLabel(Vector2(10, 10)));
	scoreLabel->setTint(Vector4(0, 0, 0, 1));
	//textLabels.push_back(scoreLabel.get());
	//activeControls.push_back(scoreLabel.get());

	energyLabel.reset(guiFactory->createTextLabel(Vector2(10, 30)));
	energyLabel->setTint(Vector4(0, 0, 0, 1));
	//activeControls.push_back(energyLabel.get());

	pauseOverlay.reset(
		guiFactory->createRectangle(Vector2(0, 0),
			Vector2(Globals::WINDOW_WIDTH, Globals::WINDOW_HEIGHT)));
	pauseOverlay->setTint(Color(1, .588, 1, .8)); //should be pinkish


	pauseLabel.reset(guiFactory->createTextLabel(Vector2::Zero, "BlackCloak"));
	pauseLabel->setText(L"Paused");
	pauseLabel->setScale(Vector2(1, 1.5));
	Vector2 size = pauseLabel->measureString();
	pauseLabel->setPosition(Vector2(
		(Globals::WINDOW_WIDTH - size.x) / 2, (Globals::WINDOW_HEIGHT - size.y) / 2));

	gameOverLabel.reset(guiFactory->createTextLabel(Vector2::Zero, "BlackCloak"));
	gameOverLabel->setText("Game Over");
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
		"BlackCloak"));
	warningLabel->setText("GET READY!");
	warningLabel->setScale(Vector2(1, 1.5));
	size = warningLabel->measureString(L"GET READY!");
	warningLabel->moveBy(-size / 2);
	//activeControls.push_back(warningLabel.get());
}

GUIOverlay::~GUIOverlay() {
	//activeControls.clear();
}

const Vector2& GUIOverlay::getPlayArea() {
	return hudBG->getSize();
}

const Vector2& GUIOverlay::getPlayPosition() {
	return hudBG->getPosition();
}


bool rInc = true;
bool gInc = false;
bool bInc = true;
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

	exitButton->update(deltaTime);
	continueButton->update(deltaTime);

}


void GUIOverlay::draw(SpriteBatch* batch) {

	hudBG->draw(batch);
	timerLabel->draw(batch);
	scoreLabel->draw(batch);
	energyLabel->draw(batch);

}

void GUIOverlay::drawPaused(SpriteBatch* batch) {

	pauseOverlay->draw(batch);
	exitButton->draw(batch);
	continueButton->draw(batch);
	pauseLabel->draw(batch);

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