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


	void update(double deltaTime, KeyboardController* keys, MouseController* mouse);
	void draw(SpriteBatch* batch);


	void loadLevel(const wchar_t* file);
	void loadMainMenu();
	void exit();

	vector<wstring> getAdapterListDescriptions();
	vector<ComPtr<IDXGIAdapter> > getAdapterList();
	vector<wstring> getDisplayModeListDescriptions(size_t adapterIndex);
	vector<DXGI_MODE_DESC> getDisplayModeList(size_t adapterIndex);
	size_t getSelectedAdapterIndex();
	size_t getSelectedDisplayMode();

private:

	Screen* currentScreen;
	Screen* lastScreen = 0;
	unique_ptr<LevelManager> levelScreen;
	unique_ptr<MenuManager> menuScreen;


	GameEngine* gameEngine;
	MouseController* mouse;
	ID3D11Device* device;
	


	
};