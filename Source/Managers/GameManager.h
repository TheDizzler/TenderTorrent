#pragma once

//#include <pugixml.hpp>
#include "LevelManager.h"
#include "MenuManager.h"



enum GameState {
	menu, paused, playing
};


class GameEngine;

/** The lowest level of class where game code should be included.
	Everything below this (GameEngine downward) should generally go unmodified. */
class GameManager {
public:
	GameManager(GameEngine* gameEngine);
	~GameManager();


	bool initializeGame(ID3D11Device* device, MouseController* mouse);


	void update(double deltaTime, BYTE keyboardState[256], MouseController* mouse);
	void draw(SpriteBatch* batch);


	void loadLevel(const wchar_t* file);
	void loadMainMenu();
	void exit();

	vector<wstring> getAdapters();

private:

	Screen* currentScreen;
	Screen* lastScreen = 0;
	unique_ptr<LevelManager> levelScreen;
	unique_ptr<MenuManager> menuScreen;


	GameEngine* gameEngine;
	MouseController* mouse;
	ID3D11Device* device;
	


	
};