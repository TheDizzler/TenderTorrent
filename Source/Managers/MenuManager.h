#pragma once

#include <vector>

#include "Screen.h"
//#include "../GameObjects/GUIObjects/Button.h"
#include "../GameObjects/GUIObjects/Dialog.h"

class MenuManager : public Screen {
public:
	MenuManager();
	~MenuManager();


	virtual void setGameManager(GameManager* game);

	virtual bool initialize(ID3D11Device* device, MouseController* mouse);
	virtual void update(double deltaTime, BYTE keyboardState[256],
		MouseController* mouse);
	virtual void draw(SpriteBatch* batch);

private:

	void confirmExit();

	unique_ptr<Dialog> exitDialog;
	unique_ptr<FontSet> menuFont;

	unique_ptr<TextLabel> test;
	unique_ptr<TextLabel> mouseLabel;

	vector<TextLabel*> textLabels;
	vector<Button*> buttons;

	GameManager* game;

};