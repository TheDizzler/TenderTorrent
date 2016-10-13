#pragma once

#include "../DXTKGui/BaseGraphics/screen.h"
#include "../DXTKGui/Controls/Dialog.h"
#include "../DXTKGui/Controls/ListBox.h"
#include "../DXTKGUI/Effects/ScreenTransitions.h"

class MenuScreen;
class MainScreen;
class ConfigScreen;

class DisplayModeItem : public ListItem {
public:
	DXGI_MODE_DESC modeDesc;

protected:
	virtual void setText();

};

class DisplayItem : public ListItem {
public:
	ComPtr<IDXGIOutput> adapterOutput;

protected:
	virtual void setText();
};

class AdapterItem : public ListItem {
public:
	ComPtr<IDXGIAdapter> adapter;

protected:
	virtual void setText();

};



class OnClickListenerAdapterList : public ListBox::OnClickListener {
public:
	OnClickListenerAdapterList(ConfigScreen* screen) : config(screen) {
	}
	virtual void onClick(ListBox* listbox, int selectedIndex) override;
private:
	ConfigScreen* config;
};

class OnClickListenerDisplayModeList : public ComboBox::OnClickListener {
public:
	OnClickListenerDisplayModeList(ConfigScreen* screen) : config(screen) {
	}
	virtual void onClick(ComboBox* combobox, int selectedIndex) override;
private:
	ConfigScreen* config;

};

class OnClickListenerFullScreenCheckBox : public CheckBox::OnClickListener {
public:
	OnClickListenerFullScreenCheckBox(ConfigScreen* screen) : config(screen) {
	}
	virtual void onClick(CheckBox* checkbox, bool isChecked) override;
private:
	ConfigScreen* config;
};

class PlayButtonListener : public Button::OnClickListener {
public:
	PlayButtonListener(GameManager* gm) : game(gm) {
	}
	virtual void onClick(Button* button) override;
private:
	GameManager* game;
};

class SettingsButtonListener : public Button::OnClickListener {
public:
	SettingsButtonListener(MainScreen* screen) : main(screen) {
	}
	virtual void onClick(Button* button) override;
private:
	MainScreen* main;
};

class OnClickListenerDialogQuitButton : public Button::OnClickListener {
public:
	OnClickListenerDialogQuitButton(MainScreen* screen) : main(screen) {
	}
	virtual void onClick(Button * button) override;
private:
	MainScreen* main;
};

class OnClickListenerExitButton : public Button::OnClickListener {
public:
	OnClickListenerExitButton(MainScreen* screen) : main(screen) {
	}
	virtual void onClick(Button* button) override;
private:
	MainScreen* main;
};

class MenuManager : public Screen {
public:
	MenuManager();
	~MenuManager();

	virtual bool initialize(ComPtr<ID3D11Device> device, MouseController* mouse) override;
	virtual void setGameManager(GameManager* game) override;
	virtual void update(double deltaTime, KeyboardController* keys, MouseController* mouse) override;
	virtual void draw(SpriteBatch* batch) override;


	virtual void pause() override;

	void openMainMenu();
	void openConfigMenu();

	unique_ptr<ScreenTransitions::ScreenTransitionManager> transitionManager;

private:

	Screen* currentScreen = NULL;
	Screen* switchTo = NULL;
	unique_ptr<MainScreen> mainScreen;
	unique_ptr<ConfigScreen> configScreen;

	GameManager* game;
};



class MenuScreen : public Screen {
	friend class SettingsButtonListener;
public:

	MenuScreen(MenuManager* manager);
	~MenuScreen();

	virtual void setGameManager(GameManager* game) override;
	virtual void pause() override;
protected:

	GameManager* game;
	MenuManager* menuManager;

	vector<GUIControl*> guiControls;
};


class ConfigScreen : public MenuScreen {
	friend class OnClickListenerAdapterList;
	friend class OnClickListenerDisplayModeList;
	friend class OnClickListenerFullScreenCheckBox;
public:
	ConfigScreen(MenuManager* manager);
	~ConfigScreen();

	virtual bool initialize(ComPtr<ID3D11Device> device, MouseController* mouse) override;
	virtual void update(double deltaTime,
		KeyboardController* keys, MouseController* mouse) override;
	virtual void draw(SpriteBatch* batch) override;

private:
	void populateDisplayList(vector<ComPtr<IDXGIOutput> > displays);
	void populateDisplayModeList(vector<DXGI_MODE_DESC> displayModes);
	TextLabel* adapterLabel;
	TextLabel* displayLabel;
	TextLabel* testLabel;


	ListBox* adapterListbox;
	ListBox* displayListbox;
	ComboBox* displayModeCombobox;
};

class MainScreen : public MenuScreen {
	friend class OnClickListenerExitButton;
	friend class OnClickListenerDialogQuitButton;
public:
	MainScreen(MenuManager* manager);
	~MainScreen();

	virtual bool initialize(ComPtr<ID3D11Device> device, MouseController* mouse) override;
	virtual void update(double deltaTime, KeyboardController* keys,
		MouseController* mouse) override;
	virtual void draw(SpriteBatch * batch) override;

private:
	unique_ptr<Dialog> exitDialog;

	void confirmExit();
};

