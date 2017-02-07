#include "../pch.h"
#pragma once

#include "../DXTKGui/Controls/Button.h"


class GUIOverlay {
public:
	GUIOverlay();
	~GUIOverlay();

	void updateWarning(double deltaTime);
	void updatePaused(double deltaTime);
	void draw(SpriteBatch* batch);
	void drawPaused(SpriteBatch* batch);
	void drawWarning(SpriteBatch* batch);

	unique_ptr<Button> exitButton;
	unique_ptr<Button> continueButton;

	unique_ptr<TextLabel> timerLabel;
	unique_ptr<TextLabel> scoreLabel;
	unique_ptr<TextLabel> energyLabel;
	unique_ptr<TextLabel> pauseLabel;
	unique_ptr<TextLabel> warningLabel;

	unique_ptr<RectangleSprite> pauseOverlay;
private:

	//vector<GUIControl* > activeControls;

	//inline void displayWarning(double deltaTime);
	//inline void displayPause(double deltaTime);
};