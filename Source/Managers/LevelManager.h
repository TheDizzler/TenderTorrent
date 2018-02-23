#pragma once

#include "GUIOverlay.h"

#include "../GameObjects/PlayerShip.h"
#include "WaveManager.h"
#include "../GameObjects/Graphics/Background.h"


class LevelManager;
class ContinueButtonListener: public Button::ActionListener {
public:
	ContinueButtonListener(LevelManager* lvlMan) : lvlManager(lvlMan){}

	virtual void onClick(Button * button) override;
	virtual void onPress(Button* button) override;
	virtual void onHover(Button* button) override;
	virtual void resetState(Button* button) override;
private:
	LevelManager* lvlManager;

};


class ExitButtonListener : public Button::ActionListener {
public:
	ExitButtonListener(LevelManager* level) : lvlManager(level) {
	}

	virtual void onClick(Button* button) override;
	virtual void onPress(Button* button) override;
	virtual void onHover(Button* button) override;
	virtual void resetState(Button* button) override;
private:
	LevelManager* lvlManager;

};


class LevelManager : public Screen {
friend class ExitButtonListener;
friend class ContinueButtonListener;
public:

	enum PlayState {
		LOADING, STARTING, PAUSED, PLAYING, FINISHED, GAMEOVER
	};

	virtual ~LevelManager();

	virtual void setGameManager(GameManager* game);

	virtual bool initialize(ComPtr<ID3D11Device> device);
	void reloadGraphicsAssets();

	bool loadLevel(ComPtr<ID3D11Device> device, const char_t* levelFileName);


	virtual void update(double deltaTime);
	virtual void draw(SpriteBatch* batch) override;
	virtual void textureDraw(SpriteBatch* batch) override;

	virtual void pause() override;

	virtual void controllerRemoved(ControllerSocketNumber controllerSlot,
		PlayerSlotNumber slotNumber) override;
	virtual void newController(shared_ptr<Joystick> newStick) override;

private:

	//xml_document levelManifest;
	xml_document shipDataDoc;

	PlayState playState = LOADING;

	GameManager* game;

	Background bgManager;
	WaveManager waveManager;

	ComPtr<ID3D11RasterizerState> rasterState;
	GUIOverlay guiOverlay;
	unique_ptr<RectangleFrame> testFrame;


	PlayerShip playerShip;




	float timeStep = 1.0f / 60.0f;
	float timeSinceLastStep = 0;
	int velocityIterations = 8;
	int positionIterations = 3;

	double totalPlayTime = 0;
	int score = 0;

	double gameOverTimer = 0;

	bool isPaused;
	bool delayedPause = false;
	//bool pauseDownLast = false;
	//double pauseDelay = 0;

	void resume();
	void exitLevel();



};