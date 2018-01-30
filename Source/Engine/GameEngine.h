#pragma once


#include "GraphicsEngine.h"
#include "Input.h"

#include "../Managers/GameManager.h"



/** The engine to connect higher level game code to the low level
	graphic and OS software. This class should be reusable for any 2D game,
	thus should not contain any game logic. */
class GameEngine : public GraphicsEngine, public Input {
public:

	virtual ~GameEngine();

	bool initEngine(HWND hwnd, HINSTANCE hInstance);
	void onAudioDeviceChange();

	virtual void reloadGraphicsAssets() override;

	void run(double time);

	void suspend();
	void resume();
	void exit();

	virtual void controllerRemoved(ControllerSocketNumber controllerSocket,
		PlayerSlotNumber slotNumber) override;
	virtual void newController(shared_ptr<Joystick> newStick) override;

	static inline bool reportError(HRESULT hr,
		wstring failMessage = L"This is SRS Error",
		wstring failTitle = L"Fatal Error",
		bool showMessageBox = false, bool dontUseGUI = false) {

		if (FAILED(hr)) {

			_com_error err(hr);
			wostringstream wss;
			wss << failMessage;
			wss << "\nHRESULT: " << err.ErrorMessage() << endl;
			if (GUIFactory::initialized && !dontUseGUI)
				GameEngine::showWarningDialog(wss.str(), failTitle);
			else if (!Globals::FULL_SCREEN && showMessageBox)
				MessageBox(NULL, wss.str().c_str(), failTitle.c_str(), MB_OK | MB_ICONERROR);

			failTitle += L" >> " + failMessage + L"\n\tHRESULT: " + err.ErrorMessage() + L"\n";
			OutputDebugString(failTitle.c_str()); // always output debug just in case
			return true;
		}

		return false;
	}

	
	static void errorMessage(wstring message, wstring title = L"Fatal Error",
		bool showMessageBox = false) {

		GameManager::errorMessage(message, title, showMessageBox);
	}

	static void showErrorDialog(wstring message, wstring title) {
		GameManager::showErrorDialog(message, title);
	}

	static void showWarningDialog(wstring message, wstring title) {
		GameManager::showWarningDialog(message, title);
	}
private:

	unique_ptr<AudioEngine> audioEngine;
	GameManager game;

	void update(double time);
	virtual void render() override;

	bool retryAudio;


};


class QuitButtonListener : public Button::ActionListener {
public:
	QuitButtonListener(GameManager* man) : mananger(man) {
	}
	virtual void onClick(Button * button) override {
		mananger->exit();
	}
	virtual void onPress(Button* button) override {
	}
	virtual void onHover(Button* button) override {
	};
	virtual void resetState(Button* button) override {
	};

	GameManager* mananger;
};
