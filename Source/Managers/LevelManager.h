#pragma once

#include "../DXTKGui/BaseGraphics/screen.h"
#include "../DXTKGui/Controls/Button.h"
#include "../GameObjects/PlayerShip.h"
#include "WaveManager.h"
#include "BackgroundManager.h"

using namespace std;



class LevelManager : public Screen {
public:

	enum PlayState {
		LOADING, STARTING, PAUSED, PLAYING
	};


	LevelManager();
	~LevelManager();

	virtual void setGameManager(GameManager* game);

	virtual bool initialize(ComPtr<ID3D11Device> device, MouseController* mouse);
	bool loadLevel(ComPtr<ID3D11Device> device, const wchar_t* file);


	virtual void update(double deltaTime, KeyboardController* keys,
		MouseController* mouse);
	virtual void draw(SpriteBatch* batch);

	virtual void pause() override;


private:

	PlayState playState = LOADING;

	GameManager* game;

	unique_ptr<BackgroundManager> bgManager;
	unique_ptr<WaveManager> waveManager;

	DIMOUSESTATE mouseLastState;

	unique_ptr<Button> exitButton;
	unique_ptr<Button> continueButton;
	//unique_ptr<FontSet> guiFont;
	//unique_ptr<FontSet> pauseFont;
	//unique_ptr<FontSet> warningFont;
	unique_ptr<TextLabel> timerLabel;
	unique_ptr<TextLabel> scoreLabel;
	unique_ptr<TextLabel> energyLabel;
	unique_ptr<TextLabel> pauseLabel;
	unique_ptr<TextLabel> warningLabel;
	vector<TextLabel*> textLabels;

	unique_ptr<PlayerShip> playerShip;

	//unique_ptr<Sprite> pauseOverlay;
	unique_ptr<RectangleSprite> pauseOverlay;


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
	void displayWarning(double deltaTime);
	void displayPause(double deltaTime);

	
};