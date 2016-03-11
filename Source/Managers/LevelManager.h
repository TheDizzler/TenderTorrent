#pragma once

//#include <tchar.h>
//#include <sstream>
//#include <strsafe.h>

#include "Screen.h"
#include "../GameObjects/PlayerShip.h"
#include "WaveManager.h"

using namespace std;


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
	GameManager* game;

	unique_ptr<WaveManager> waveManager;

	DIMOUSESTATE mouseLastState;

	Sprite* mouse;
	unique_ptr<FontSet> guiFont;
	unique_ptr<FontSet> pauseFont;

	unique_ptr<TextLabel> timerLabel;
	unique_ptr<TextLabel> scoreLabel;
	unique_ptr<TextLabel> energyLabel;
	unique_ptr<TextLabel> pauseLabel;
	vector<TextLabel* > textLabels;

	unique_ptr<PlayerShip> playerShip;

	float timeStep = 1.0f / 60.0f;
	float timeSinceLastStep = 0;
	int velocityIterations = 8;
	int positionIterations = 3;


	int score = 0;

	bool isPaused;
};