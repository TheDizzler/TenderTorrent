#pragma once

#include "LevelSelectScreen.h"
#include "../../DXTKGUI/Effects/ScreenTransitions.h"



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


class OnClickListenerDisplayList : public ListBox::ActionListener {
public:
	OnClickListenerDisplayList(ConfigScreen* screen) : config(screen) {
	}
	virtual void onClick(ListBox * listbox, UINT selectedItemIndex) override;
	virtual void onHover(ListBox * listbox, short hoveredItemIndex) override;
private:
	ConfigScreen* config;
};

class OnClickListenerAdapterList : public ListBox::ActionListener {
public:
	OnClickListenerAdapterList(ConfigScreen* screen) : config(screen) {
	}
	virtual void onClick(ListBox* listbox, UINT selectedItemIndex) override;
	virtual void onHover(ListBox* listbox, short hoveredItemIndex) override;
private:
	ConfigScreen* config;
};

class OnClickListenerDisplayModeList : public ComboBox::ActionListener {
public:
	OnClickListenerDisplayModeList(ConfigScreen* screen) : config(screen) {
	}
	virtual void onClick(ComboBox* combobox, UINT selectedIndex) override;
	virtual void onHover(ComboBox* listbox, short hoveredItemIndex) override;
private:
	ConfigScreen* config;

};

class OnClickListenerFullScreenCheckBox : public CheckBox::ActionListener {
public:
	OnClickListenerFullScreenCheckBox(ConfigScreen* screen) : config(screen) {
	}
	virtual void onClick(CheckBox* checkbox, bool isChecked) override;
	virtual void onHover(CheckBox* checkbox, bool isChecked) override;
private:
	ConfigScreen* config;
};

class PlayButtonListener : public Button::ActionListener {
public:
	PlayButtonListener(MainScreen* screen) : main(screen) {
	}
	virtual void onClick(Button* button) override;
	virtual void onPress(Button* button) override;
	virtual void onHover(Button* button) override;
	virtual void resetState(Button* button) override;
private:
	MainScreen* main;
};

class SettingsButtonListener : public Button::ActionListener {
public:
	SettingsButtonListener(MainScreen* screen) : main(screen) {
	}
	virtual void onClick(Button* button) override;
	virtual void onPress(Button* button) override;
	virtual void onHover(Button* button) override;
	virtual void resetState(Button* button) override;
private:
	MainScreen* main;
};

class OnClickListenerDialogQuitButton : public Button::ActionListener {
public:
	OnClickListenerDialogQuitButton(MainScreen* screen) : main(screen) {
	}
	virtual void onClick(Button* button) override;
	virtual void onPress(Button* button) override;
	virtual void onHover(Button* button) override;
	virtual void resetState(Button* button) override;
private:
	MainScreen* main;
};

class OnClickListenerExitButton : public Button::ActionListener {
public:
	OnClickListenerExitButton(MainScreen* screen) : main(screen) {
	}
	virtual void onClick(Button* button) override;
	virtual void onPress(Button* button) override;
	virtual void onHover(Button* button) override;
	virtual void resetState(Button* button) override;
private:
	MainScreen* main;
};

class BackButtonListener : public Button::ActionListener {
public:
	BackButtonListener(ConfigScreen* screen) : configScreen(screen) {
	}
	virtual void onClick(Button* button) override;
	virtual void onPress(Button* button) override;
	virtual void onHover(Button* button) override;
	virtual void resetState(Button* button) override;
private:
	ConfigScreen* configScreen;
};

class MenuManager : public Screen {
public:
	MenuManager();
	virtual ~MenuManager();

	virtual bool initialize(ComPtr<ID3D11Device> device) override;
	virtual void setGameManager(GameManager* game) override;
	void reloadGraphicsAssets();

	virtual void update(double deltaTime) override;
	virtual void draw(SpriteBatch* batch) override;
	virtual void textureDraw(SpriteBatch* batch) override;

	virtual void pause() override;

	virtual void controllerRemoved(ControllerSocketNumber controllerSlot,
		PlayerSlotNumber slotNumber) override;
	virtual void newController(shared_ptr<Joystick> newStick) override;

	void openMainMenu();
	void openConfigMenu();
	void openLevelSelectScreen();
	void loadLevel(string levelXMLFile);

private:

	ScreenTransitions::ScreenTransitionManager transitionManager;
	Screen* currentScreen = NULL;
	Screen* switchTo = NULL;
	unique_ptr<MainScreen> mainScreen;
	unique_ptr<ConfigScreen> configScreen;
	unique_ptr<LevelSelectScreen> levelSelectScreen;

	GameManager* game;
};



class MenuScreen : public Screen {
public:

	MenuScreen(MenuManager* manager);
	virtual ~MenuScreen();

	virtual void setGameManager(GameManager* game) override;
	virtual void reloadGraphicsAssets() = 0;
	virtual void pause() override;

	virtual void controllerRemoved(ControllerSocketNumber controllerSlot,
		PlayerSlotNumber slotNumber) override;
	virtual void newController(shared_ptr<Joystick> newStick) override;


	MenuManager* menuManager;
protected:

	GameManager* game;
	//vector<unique_ptr<GUIControl>> guiControls;
	vector<GUIControl*> guiControls;
};


class ConfigScreen : public MenuScreen {
	friend class OnClickListenerDisplayList;
	friend class OnClickListenerAdapterList;
	friend class OnClickListenerDisplayModeList;
	friend class OnClickListenerFullScreenCheckBox;
public:
	ConfigScreen(MenuManager* manager);
	virtual ~ConfigScreen();

	virtual bool initialize(ComPtr<ID3D11Device> device) override;
	virtual void reloadGraphicsAssets() override;

	virtual void update(double deltaTime) override;
	virtual void draw(SpriteBatch* batch) override;

	virtual void textureDraw(SpriteBatch* batch) override;

	virtual void controllerRemoved(ControllerSocketNumber controllerSlot,
		PlayerSlotNumber slotNumber) override;
	virtual void newController(shared_ptr<Joystick> newStick) override;

private:
	void populateDisplayList(vector<ComPtr<IDXGIOutput> > displays);
	void populateDisplayModeList(vector<DXGI_MODE_DESC> displayModes);

	unique_ptr<TexturePanel> texturePanel;
	bool refreshTexture = true;

	TextLabel* adapterLabel;
	TextLabel* displayLabel;
	TextLabel* testLabel;

	ListBox* adapterListbox;
	ListBox* displayListbox;
	ComboBox* displayModeCombobox;

	Spinner* testSpinner;
	TextLabel* spinnerLabel;
};

class MainScreen : public MenuScreen {
	friend class OnClickListenerExitButton;
	friend class OnClickListenerDialogQuitButton;
public:
	MainScreen(MenuManager* manager);
	virtual ~MainScreen();

	virtual bool initialize(ComPtr<ID3D11Device> device) override;

	virtual void reloadGraphicsAssets() override;

	virtual void update(double deltaTime) override;
	virtual void draw(SpriteBatch* batch) override;

private:
	unique_ptr<PromptDialog> exitDialog;

	void confirmExit();
};

