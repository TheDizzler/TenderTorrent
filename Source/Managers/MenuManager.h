#pragma once

#include <vector>

#include "Screen.h"
#include "../GameObjects/GUIObjects/Dialog.h"
#include "../GameObjects/GUIObjects/ListBox.h"
#include "../GameObjects/GUIObjects/TextButton.h"


class MenuScreen;
class MainScreen;
class ConfigScreen;

class MenuManager : public Screen {
public:
	MenuManager();
	~MenuManager();


	virtual void setGameManager(GameManager* game);

	virtual bool initialize(ID3D11Device* device, MouseController* mouse);
	virtual void update(double deltaTime, BYTE keyboardState[256],
		MouseController* mouse);
	virtual void draw(SpriteBatch* batch);

	void openMainMenu();
	void openConfigMenu();

	unique_ptr<FontSet> menuFont;

private:

	Screen* currentScreen = NULL;
	unique_ptr<MainScreen> mainScreen;
	unique_ptr<ConfigScreen> configScreen;


	GameManager* game;
};



class MenuScreen : public Screen {
public:

	MenuScreen(MenuManager* manager, FontSet* fontSet);
	~MenuScreen();

	// Inherited via Screen
	virtual void setGameManager(GameManager* game) override;

protected:

	GameManager* game;
	MenuManager* menuManager;
	FontSet* menuFont;

	vector<TextLabel*> textLabels;
	vector<TextButton*> buttons;

	vector<ListBox*> listBoxes; 

};


class ConfigScreen : public MenuScreen {
public:
	ConfigScreen(MenuManager* manager, FontSet* fontSet);
	~ConfigScreen();

	// Inherited via MenuScreen
	virtual bool initialize(ID3D11Device * device, MouseController * mouse) override;
	virtual void update(double deltaTime, BYTE keyboardState[256], MouseController * mouse) override;
	virtual void draw(SpriteBatch * batch) override;

private:
	
};

class MainScreen : public MenuScreen {
public:
	MainScreen(MenuManager* manager, FontSet* fontSet);
	~MainScreen();

	// Inherited via MenuScreen
	virtual bool initialize(ID3D11Device * device, MouseController * mouse) override;
	virtual void update(double deltaTime, BYTE keyboardState[256], MouseController * mouse) override;
	virtual void draw(SpriteBatch * batch) override;

private:
	unique_ptr<Dialog> exitDialog;
	unique_ptr<TextLabel> test;
	unique_ptr<TextLabel> mouseLabel;

	void confirmExit();

	
};

