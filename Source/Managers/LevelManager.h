#include "../pch.h"
#pragma once

#include "GUIOverlay.h"

#include "../GameObjects/PlayerShip.h"
#include "WaveManager.h"
#include "BackgroundManager.h"


class LevelManager;
class ContinueButtonListener: public Button::OnClickListener {
public:
	ContinueButtonListener(LevelManager* lvlMan) : lvlManager(lvlMan){}

	virtual void onClick(Button * button) override;
private:
	LevelManager* lvlManager;

};


class ExitButtonListener : public Button::OnClickListener {
public:
	ExitButtonListener(LevelManager* level) : lvlManager(level) {
	}

	virtual void onClick(Button* button) override;
private:
	LevelManager* lvlManager;

};


class LevelManager : public Screen {
friend class ExitButtonListener;
friend class ContinueButtonListener;
public:

	enum PlayState {
		LOADING, STARTING, PAUSED, PLAYING
	};


	LevelManager();
	~LevelManager();

	virtual void setGameManager(GameManager* game);

	virtual bool initialize(ComPtr<ID3D11Device> device, shared_ptr<MouseController> mouse);


	bool loadLevel(ComPtr<ID3D11Device> device, const char_t* levelFileName);


	virtual void update(double deltaTime, shared_ptr<MouseController> mouse);
	virtual void draw(SpriteBatch* batch);

	virtual void pause() override;


private:

	unique_ptr<xml_document> levelManifest;

	PlayState playState = LOADING;

	GameManager* game;

	unique_ptr<BackgroundManager> bgManager;
	unique_ptr<WaveManager> waveManager;

	unique_ptr<GUIOverlay> guiOverlay;

	/*unique_ptr<Button> exitButton;
	unique_ptr<Button> continueButton;

	unique_ptr<TextLabel> timerLabel;
	unique_ptr<TextLabel> scoreLabel;
	unique_ptr<TextLabel> energyLabel;
	unique_ptr<TextLabel> pauseLabel;
	unique_ptr<TextLabel> warningLabel;

	unique_ptr<RectangleSprite> pauseOverlay;*/

	unique_ptr<PlayerShip> playerShip;




	float timeStep = 1.0f / 60.0f;
	float timeSinceLastStep = 0;
	int velocityIterations = 8;
	int positionIterations = 3;

	double totalPlayTime = 0;
	int score = 0;

	bool isPaused;
	bool delayedPause = false;
	bool pauseDownLast = false;
	//double pauseDelay = 0;

	void resume();
	void exitLevel();



};