#pragma once

#include <vector>

#include "Screen.h"
#include "../GameObjects/GUIObjects/Dialog.h"

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

	unique_ptr<FontSet> menuFont;

private:

	Screen* currentScreen = NULL;
	unique_ptr<MainScreen> mainScreen;
	unique_ptr<ConfigScreen> configScreen;


	GameManager* game;
};


class MainScreen : public Screen {

public:

	MainScreen(MenuManager* manager, FontSet* fontSet);
	~MainScreen();

	// Inherited via Screen
	virtual bool initialize(ID3D11Device* device, MouseController* mouse) override;
	virtual void setGameManager(GameManager* game) override;
	virtual void update(double deltaTime, BYTE keyboardState[256], MouseController* mouse) override;
	virtual void draw(SpriteBatch* batch) override;

private:

	GameManager* game;
	MenuManager* manager;
	FontSet* menuFont;
	vector<TextLabel*> textLabels;
	vector<Button*> buttons;

	unique_ptr<Dialog> exitDialog;
	unique_ptr<TextLabel> test;
	unique_ptr<TextLabel> mouseLabel;

	void confirmExit();
};


class ConfigScreen : Screen {
public:
	ConfigScreen(MenuManager* manager, FontSet* fontSet);
	~ConfigScreen();

	// Inherited via Screen
	virtual bool initialize(ID3D11Device* device, MouseController* mouse) override;
	virtual void setGameManager(GameManager* game) override;
	virtual void update(double deltaTime, BYTE keyboardState[256], MouseController* mouse) override;
	virtual void draw(SpriteBatch * batch) override;

private:

	GameManager* game;
	MenuManager* manager;
	FontSet* menuFont;
	vector<TextLabel*> textLabels;
	vector<Button*> buttons;

	unique_ptr<Dialog> exitDialog;
	unique_ptr<TextLabel> test;
	unique_ptr<TextLabel> mouseLabel;

};