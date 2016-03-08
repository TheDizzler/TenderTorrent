#pragma once

#include <tchar.h>
#include <sstream>
#include <strsafe.h>

#include "../LevelController/TextLabel.h"
#include "../GameObjects/PlayerShip.h"
#include "WaveManager.h"

using namespace std;


class LevelManager {
public:
	LevelManager(ID3D11Device* device);
	~LevelManager();

	bool initStage(ID3D11Device* device);

	void update(double deltaTime, BYTE keyboardState[256], DIMOUSESTATE mouseCurrentState, Vector2 mousePos);
	void draw(SpriteBatch* batch);

	void pauseScreen(double deltaTime, SpriteBatch* batch);

private:

	unique_ptr<WaveManager> waveManager;

	DIMOUSESTATE mouseLastState;

	unique_ptr<Sprite> mouse;
	unique_ptr<FontSet> arial;
	unique_ptr<FontSet> pauseFont;


	unique_ptr<PlayerShip> playerShip;


	std::vector<TextLabel* > textLabels;

	unique_ptr<TextLabel> timerLabel;
	unique_ptr<TextLabel> scoreLabel;
	unique_ptr<TextLabel> energyLabel;
	unique_ptr<TextLabel> pauseLabel;

	float timeStep = 1.0f / 60.0f;
	float timeSinceLastStep = 0;
	int velocityIterations = 8;
	int positionIterations = 3;


	int score = 0;


};