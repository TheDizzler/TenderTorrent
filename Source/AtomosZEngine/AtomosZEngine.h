#if !defined(ATOMOSZ_ENGINE_H)
/*****************************************************************************
*	$Note:	$
*	$Author: TDGillon $
*	$Notice: (C) Copyright 2023 by AtomosZ Industries. All rights reserved. $
*****************************************************************************/

#include "AtomosZ_Platform.h"
struct MemoryArena
{
	memory_index size;
	uint8* base;
	memory_index used;
};


inline void InitializeMemoryArena(MemoryArena* arena, memory_index size, void* base)
{
	arena->size = size;
	arena->base = (uint8*)base;
	arena->used = 0;
}

#define PushArray(arena, count, type) (type*)PushSize_(arena, (count)*sizeof(type))
#define PushStruct(arena, type) (type*)PushSize_(arena, sizeof(type))
inline void* PushSize_(MemoryArena* arena, memory_index size)
{
	Assert(arena->used + size <= arena->size);
	void* result = arena->base + arena->used;
	arena->used += size;
	return result;
}

#define ZeroStruct(instance) ZeroSize(sizeof(instance), &(instance))

inline void ZeroSize(memory_index size, void* ptr)
{
	uint8* byte = (uint8*)ptr;
	while (size--)
	{
		*byte++ = 0;
	}
}


/// <summary>
/// This must be initialized by the graphics renderer.
/// </summary>
struct TransientState
{
	MemoryArena transArena;
	void* drawBatchMemory;
};

struct GameState
{
	MemoryArena memoryArena;
};

#if ATOMOSZ_NOTIMPLEMNTEDYET
#include "GraphicsEngine.h"
#include "Input.h"
#include "UserInput.h"

#include "../Managers/GameManager.h"




/** The engine to connect higher level game code to the low level
	graphic and OS software. This class should be reusable for any 2D game,
	thus should not contain any game logic. */
class GameEngine : public GraphicsEngine, public Input
{
public:

	virtual ~GameEngine();

	bool initEngine(HWND hwnd, HINSTANCE hInstance, GameInput* gameInput);
	void onAudioDeviceChange();

	virtual void reloadGraphicsAssets() override;

	void Run(GameInput* gameInput);

	void suspend();
	void resume();
	void exit();

	virtual void controllerRemoved(ControllerSocketNumber controllerSocket,
		PlayerSlotNumber slotNumber) override;
	virtual void newController(shared_ptr<Joystick> newStick) override;

	static inline bool reportError(HRESULT hr,
		wstring failMessage = L"This is SRS Error",
		wstring failTitle = L"Fatal Error",
		bool showMessageBox = false, bool dontUseGUI = false)
	{

		if (FAILED(hr))
		{

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
		bool showMessageBox = false)
	{

		GameManager::errorMessage(message, title, showMessageBox);
	}

	static void showErrorDialog(wstring message, wstring title)
	{
		GameManager::showErrorDialog(message, title);
	}

	static void showWarningDialog(wstring message, wstring title)
	{
		GameManager::showWarningDialog(message, title);
	}
private:

	unique_ptr<AudioEngine> audioEngine;
	GameManager game;

	void Update(GameInput* gameInput);
	virtual void render() override;

	bool retryAudio;
};


class QuitButtonListener : public Button::ActionListener
{
public:
	QuitButtonListener(GameManager* man) : mananger(man)
	{}
	virtual void onClick(Button* button) override
	{
		mananger->exit();
	}
	virtual void onPress(Button* button) override
	{}
	virtual void onHover(Button* button) override
	{};
	virtual void resetState(Button* button) override
	{};

	GameManager* mananger;
};
#endif


#define ATOMOSZ_ENGINE_H
#endif