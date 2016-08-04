#pragma once

#include "Screen.h"
#include "../GameObjects/GUIObjects/TextButton.h"
#include "../GameObjects/PlayerShip.h"
#include "WaveManager.h"
#include "BackgroundManager.h"

using namespace std;

enum PlayState {
	LOADING, STARTING, PAUSED, PLAYING
};


class LevelManager : public Screen {
public:
	LevelManager();
	~LevelManager();

	virtual void setGameManager(GameManager* game);

	virtual bool initialize(ID3D11Device* device, MouseController* mouse);
	bool loadLevel(ID3D11Device* device, const wchar_t* file);


	virtual void update(double deltaTime, BYTE keyboardState[256],
		MouseController* mouse);
	virtual void draw(SpriteBatch* batch);


private:

	PlayState playState = LOADING;

	GameManager* game;

	unique_ptr<BackgroundManager> bgManager;
	unique_ptr<WaveManager> waveManager;

	DIMOUSESTATE mouseLastState;

	unique_ptr<TextButton> exitButton;
	unique_ptr<TextButton> continueButton;
	unique_ptr<FontSet> guiFont;
	unique_ptr<FontSet> pauseFont;
	unique_ptr<FontSet> warningFont;
	unique_ptr<TextLabel> timerLabel;
	unique_ptr<TextLabel> scoreLabel;
	unique_ptr<TextLabel> energyLabel;
	unique_ptr<TextLabel> pauseLabel;
	unique_ptr<TextLabel> warningLabel;
	vector<TextLabel* > textLabels;

	unique_ptr<PlayerShip> playerShip;

	unique_ptr<Sprite> pauseOverlay;


	float timeStep = 1.0f / 60.0f;
	float timeSinceLastStep = 0;
	int velocityIterations = 8;
	int positionIterations = 3;

	double totalPlayTime = 0;
	int score = 0;

	bool isPaused;
	bool pauseDownLast = false;
	//double pauseDelay = 0;
	void displayWarning(double deltaTime);
	void displayPause(double deltaTime);
};