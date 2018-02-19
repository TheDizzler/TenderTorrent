#pragma once

#include "../../DXTKGui/Controls/Button.h"


class GUIOverlay {
public:
	GUIOverlay();
	virtual ~GUIOverlay();

	void reloadGraphicsAssets();

	const Vector2 getPlayArea();
	const Vector2 getPlayPosition();

	void update(double deltaTime);
	void updatePaused(double deltaTime);
	void updateWarning(double deltaTime);
	void updateGameOver(double deltaTime);

	void draw(SpriteBatch* batch);
	void drawPaused(SpriteBatch* batch);
	void drawWarning(SpriteBatch* batch);
	void drawGameOver(SpriteBatch* batch);

	unique_ptr<Button> exitButton;
	unique_ptr<Button> continueButton;

	unique_ptr<TextLabel> timerLabel;
	unique_ptr<TextLabel> scoreLabel;
	unique_ptr<TextLabel> energyLabel;
	unique_ptr<TextLabel> pauseLabel;
	unique_ptr<TextLabel> warningLabel;
	unique_ptr<TextLabel> gameOverLabel;
	unique_ptr<TextLabel> fpsLabel;

	//unique_ptr<TextLabel> mouseLoc;
	//unique_ptr<TextLabel> cameraLoc;
	//unique_ptr<TextLabel> bulletCount;
	//unique_ptr<TextLabel> enemyCount;


	unique_ptr<RectangleSprite> pauseOverlay;
private:

	unique_ptr<RectangleSprite> hudBG;
	unique_ptr<RectangleFrame> hudBorder;

	//inline void displayWarning(double deltaTime);
	//inline void displayPause(double deltaTime);
};