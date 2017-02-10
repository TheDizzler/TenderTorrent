#include "../pch.h"
#pragma once

#include "../DXTKGui/Controls/Button.h"


class GUIOverlay {
public:
	GUIOverlay();
	~GUIOverlay();

	const Vector2& getPlayArea();
	const Vector2& getPlayPosition();


	void updateWarning(double deltaTime);
	void updatePaused(double deltaTime);
	//void drawGameOver(double deltaTime);
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

	unique_ptr<RectangleSprite> pauseOverlay;
private:

	unique_ptr<RectangleSprite> hudBG;
	//vector<GUIControl* > activeControls;

	//inline void displayWarning(double deltaTime);
	//inline void displayPause(double deltaTime);
};