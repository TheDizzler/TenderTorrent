#pragma once

//#include <tchar.h>
//#include <sstream>
//#include <strsafe.h>

#include "Screen.h"
#include "../GameObjects/GUIObjects/Button.h"
#include "../GameObjects/PlayerShip.h"
#include "WaveManager.h"

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

	virtual void update(double deltaTime, BYTE keyboardState[256],
		MouseController* mouse);
	virtual void draw(SpriteBatch* batch);


private:

	PlayState playState = LOADING;

	GameManager* game;

	unique_ptr<WaveManager> waveManager;

	DIMOUSESTATE mouseLastState;

	unique_ptr<Button> exitButton;
	unique_ptr<Button> continueButton;
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