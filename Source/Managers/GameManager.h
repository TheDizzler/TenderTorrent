#include "../pch.h"
#pragma once

#include "GFXAssetManager.h"

#include "LevelManager.h"
#include "MenuManager.h"



class GameEngine;

/** The lowest level of class where game code should be included.
	Everything below this (GameEngine downward) should generally go unmodified. */
class GameManager {
public:
	GameManager(GameEngine* gameEngine);
	~GameManager();


	bool initializeGame(HWND hwnd, ComPtr<ID3D11Device> device, MouseController* mouse);


	void update(double deltaTime, KeyboardController* keys, MouseController* mouse);
	void draw(SpriteBatch* batch);


	void loadLevel(const char_t* levelName);
	void loadMainMenu();

	void pause();
	void exit();

	vector<ComPtr<IDXGIAdapter> > getAdapterList();
	vector<ComPtr<IDXGIOutput> > getDisplayList();
	vector<ComPtr<IDXGIOutput> > getDisplayListFor(size_t displayIndex);
	vector<ComPtr<IDXGIOutput> > getDisplayListFor(ComPtr<IDXGIAdapter> adapter);
	vector<DXGI_MODE_DESC> getDisplayModeList(size_t displayIndex);
	vector<DXGI_MODE_DESC> getDisplayModeList(ComPtr<IDXGIOutput> display);

	bool setAdapter(size_t adapterIndex);
	bool setDisplayMode(size_t displayModeIndex);
	bool setFullScreen(bool isFullScreen);

	size_t getSelectedAdapterIndex();
	size_t getSelectedDisplayIndex();
	size_t getSelectedDisplayModeIndex();


	static unique_ptr<GUIFactory> guiFactory;
	static unique_ptr<GFXAssetManager> gfxAssets;
private:

	Screen* currentScreen = 0;
	Screen* lastScreen = 0;
	unique_ptr<LevelManager> levelScreen;
	unique_ptr<MenuManager> menuScreen;
	unique_ptr<xml_document> docAssMan;

	GameEngine* gameEngine;
	MouseController* mouse;
	ComPtr<ID3D11Device> device;
	

	
	
};