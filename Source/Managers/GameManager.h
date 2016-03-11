#pragma once

#include "LevelManager.h"
#include "MenuManager.h"


enum GameState {
	menu, paused, playing
};

/** The lowest level of class where game code should be included.
	Everything below this (GameEngine downward) should generally go unmodified. */
class GameManager {
public:
	GameManager();
	~GameManager();


	bool initializeGame(ID3D11Device* device, MouseController* mouse);


	void update(double deltaTime, BYTE keyboardState[256], MouseController* mouse);
	void draw(SpriteBatch* batch);

private:

	unique_ptr<Screen> currentScreen;
};