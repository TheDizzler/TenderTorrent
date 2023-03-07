/*****************************************************************************
*	$Note:	Slowly rebuilding this to a C standard (i.e. removing all the
*			C++ garbage) $
*	$Author: TDGillon $
*	$Notice: (C) Copyright 2023 by AtomosZ Industries. All rights reserved. $
*****************************************************************************/


#include "AtomosZ_Platform.h"

#include <windowsx.h>
#include <windows.h>
#include <stdio.h>		// _snprintf_s
#include <malloc.h>
#include <Dbt.h>		// DBT_DEVICEREMOVECOMPLETE
#include <comdef.h>		// for _com_error

//#include <Ksmedia.h>	// KSCATEGORY_AUDIO...but it doesn't work! Need Xaudio dll?
//#include <winuser.h>
//#include <fstream>	// wofstream

//#include <sstream>	// wostringstream, wstring, !Conflicts with stdio.h!

#include "StringHelper.h"
#include "Win32_AtomosZ.h"
#include "Win32_DirectXTK.h"


global bool running;

//GameEngine gameEngine;

global HWND hwnd;
global GUID guidHid;

/// <summary>
/// Ticks per second.
/// </summary>
global int64 perfCountFrequency;
/// <summary>
/// Counter when program started.
/// </summary>
global LARGE_INTEGER programStart;
global bool32 sleepIsGranular;
global float targetSecondsPerFrame;
global WINDOWPLACEMENT windowPos = {sizeof(windowPos)};
global WindowDimension initialWindowSize = WindowDimension{1280 + 22, 1024 + 56};

/** If using joysticks this is required. */
int RegisterControllers();
#if 0
/** Creates a list of all attached USB devices. */
int GetInputDeviceInfo(bool writeToFile, std::wstring filename = L"USB Devices.txt");
#endif


internal bool32 IsFullScreen()
{
	bool32 result;
	DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);
	result = dwStyle & WS_OVERLAPPEDWINDOW;
	return !result;
}

// This follows Raymond Chen's prescription for fullscreen toggling
//	https://devblogs.microsoft.com/oldnewthing/20100412-00/?p=14353
internal void ToggleFullScreen()
{
	DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);
	if (dwStyle & WS_OVERLAPPEDWINDOW)
	{	// was not fullscreen
		MONITORINFO monitorInfo = {sizeof(monitorInfo)};
		if (GetWindowPlacement(hwnd, &windowPos) &&
			GetMonitorInfo(MonitorFromWindow(hwnd,
				MONITOR_DEFAULTTOPRIMARY), &monitorInfo))
		{
			SetWindowLong(hwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
			SetWindowPos(hwnd, HWND_TOP,
				monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top,
				monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
				monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
				SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
	}
	else
	{	// was fullscreen
		SetWindowLong(hwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(hwnd, &windowPos);
		SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
}



global bool ReportError(HRESULT hr,
	char* failMessage = "This is SRS Error",
	char* failTitle = "Fatal Error")
{
	if (FAILED(hr))
	{
		_com_error err(hr);
		Assert(StringLength(failMessage) + StringLength(failTitle) + StringLength((char*)err.ErrorMessage()) < 512);
		char mbuffer[512];
		_snprintf_s(mbuffer, sizeof(mbuffer),
			"%s >> %s\n\tHRESULT: %s", failTitle, failMessage, (char*)err.ErrorMessage());
		OutputDebugStringA(mbuffer);

		if (IsFullScreen())
			ToggleFullScreen();
		MessageBox(NULL, failMessage, failTitle, MB_OK | MB_ICONERROR);

		return true;
	}

	return false;
}

/** If game set to FullScreen, Windows MessageBox will never display even
	if showMessageBox set to TRUE. */
global void DebugMessage(char* message, char* title)
{
	Assert(StringLength(message) + StringLength(title) < 512);

	char mbuffer[512];
	_snprintf_s(mbuffer, sizeof(mbuffer), "%s >> %s", title, message);
	OutputDebugStringA(mbuffer);

	if (IsFullScreen())
	{
		ToggleFullScreen();
	}

	MessageBox(NULL, message, title, MB_OK | MB_ICONERROR);
}

global void DebugMessage(char* message)
{
	Assert(StringLength(message) < 512);

	char mbuffer[512];
	_snprintf_s(mbuffer, sizeof(mbuffer), message);
	OutputDebugStringA(mbuffer);
}

global void DebugMessage(WCHAR* message, WCHAR* title)
{
	OutputDebugStringW(message);

	if (IsFullScreen())
	{
		ToggleFullScreen();
	}

	MessageBoxW(NULL, message, title, MB_OK | MB_ICONERROR);
}

global void DebugMessage(WCHAR* message)
{
	OutputDebugStringW(message);
}




internal void Win32GetEXEFilename(Win32State* win32State)
{
	// Never use MAX_PATH in code that is user-facing, because it can
	// be dangerous and lead to bad results.
	DWORD sizeOfFileName = GetModuleFileNameA(0,
		win32State->exeFileName, sizeof(win32State->exeFileName));
	win32State->onePastLastExeFileNameSlash = win32State->exeFileName;
	for (char* scan = win32State->exeFileName; *scan; ++scan)
	{
		if (*scan == '\\')
			win32State->onePastLastExeFileNameSlash = scan + 1;
	}
}

internal void Win32BuildEXEPathFilename(Win32State* win32State, char* filename,
	int destCount, char* dest)
{
	CatStrings(win32State->onePastLastExeFileNameSlash - win32State->exeFileName,
		win32State->exeFileName,
		StringLength(filename), filename,
		destCount, dest);
}




inline LARGE_INTEGER Win32GetCurrentTime()
{
	LARGE_INTEGER result;
	QueryPerformanceCounter(&result);
	return result;
}


inline float Win32GetSecondsElapsed(LARGE_INTEGER start, LARGE_INTEGER end)
{
	float tickCount = (float)(end.QuadPart - start.QuadPart) / (float)perfCountFrequency;
	return tickCount;
}


/// <summary>
/// This will remove full screen so it is somewhat neccessary.
/// </summary>
inline void ReleaseResources(HDEVNOTIFY newInterface)
{
	if (IsFullScreen())
		ChangeDisplaySettings(NULL, 0); // I think this is necessary

	if (newInterface)
		UnregisterDeviceNotification(newInterface);
}


DEBUG_PLATFORM_FREE_FILE_MEMORY(DebugPlatformFreeFileMemory)
{
	if (memory)
	{
		VirtualFree(memory, 0, MEM_RELEASE);
	}
}

DEBUG_PLATFORM_READ_ENTIRE_FILE(DebugPlatformReadEntireFile)
{
	DebugReadFileResult result = {};

	HANDLE fileHandle = CreateFileA(filename, GENERIC_READ,
		FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);

	if (fileHandle == INVALID_HANDLE_VALUE)
	{
	 // @TODO: Logging
		return result;
	}

	LARGE_INTEGER fileSize;
	if (!GetFileSizeEx(fileHandle, &fileSize))
	{
		// @TODO: Logging
		CloseHandle(fileHandle);
		return result;
	}


	result.contentsSize = SafeTruncateUInt64(fileSize.QuadPart);
	result.contents = VirtualAlloc(0, result.contentsSize,
		MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!result.contents)
	{
		// @TODO: Logging
		CloseHandle(fileHandle);
		result.contents = 0;
		result.contentsSize = 0;
		return result;
	}

	DWORD bytesRead;
	if (!ReadFile(fileHandle, result.contents, result.contentsSize, &bytesRead, 0)
		|| result.contentsSize != bytesRead)
	{
		// @TODO: Logging
		DebugPlatformFreeFileMemory(thread, result.contents);
		result.contents = 0;
		result.contentsSize = 0;
	}


	CloseHandle(fileHandle);

	return result;
}


DEBUG_PLATFORM_WRITE_ENTIRE_FILE(DebugPlatformWriteEntireFile)
{
	HANDLE fileHandle = CreateFileA(filename, GENERIC_WRITE,
		0, 0, CREATE_ALWAYS, 0, 0);

	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		// @TODO: Logging
		return 0;
	}

	DWORD bytesWritten;
	bool32 result = false;
	if (WriteFile(fileHandle, memory, memorySize, &bytesWritten, 0))
	{
		result = bytesWritten == memorySize;
	}
	else
	{
		// @TODO: Logging
	}

	CloseHandle(fileHandle);

	return result;
}


internal FILETIME Win32GetLastWriteTime(char* filename)
{
	FILETIME lastWriteTime = {};

#if 0
	WIN32_FIND_DATA findData;
	HANDLE findHandle = FindFirstFileA(filename, &findData);
	if (findHandle != INVALID_HANDLE_VALUE)
	{
		lastWriteTime = findData.ftLastWriteTime;
		FindClose(findHandle);
	}
#endif
	WIN32_FILE_ATTRIBUTE_DATA data;
	if (GetFileAttributesEx(filename, GetFileExInfoStandard, &data))
		lastWriteTime = data.ftLastWriteTime;
	return lastWriteTime;
}

internal Win32GameCode Win32LoadGameCode(char* sourceDLLName, char* tempDLLName, char* lockFilename)
{
	Win32GameCode gameCode = {};

	WIN32_FILE_ATTRIBUTE_DATA ignored;
	if (!GetFileAttributesEx(lockFilename, GetFileExInfoStandard, &ignored))
	{
		gameCode.dllLastWriteTime = Win32GetLastWriteTime(sourceDLLName);

		FILETIME lastWriteTime = Win32GetLastWriteTime(sourceDLLName);
		bool32 result = CopyFile(sourceDLLName, tempDLLName, FALSE);

		gameCode.gameCodeDLL = LoadLibraryA(tempDLLName);
		if (gameCode.gameCodeDLL)
		{
			/*gameCode.GetSoundSamples
				= (Game_Get_Sound_Samples*)GetProcAddress(gameCode.gameCodeDLL, "GameGetSoundSamples");*/
			gameCode.UpdateAndRender
				= (Game_Update_And_Render*)GetProcAddress(gameCode.gameCodeDLL, "GameUpdateAndRender");

			gameCode.Initialize
				= (Game_Initialize*)GetProcAddress(gameCode.gameCodeDLL, "GameInitialize");

			gameCode.isValid = /*gameCode.GetSoundSamples &&*/ gameCode.UpdateAndRender && gameCode.Initialize;
		}
	}

	if (!gameCode.isValid)
	{
		gameCode.UpdateAndRender = 0;
		gameCode.Initialize = 0;
		//gameCode.GetSoundSamples = 0;
	}

	return gameCode;
}

internal void Win32UnloadGameCode(Win32GameCode* gameCode)
{
	if (gameCode->gameCodeDLL)
	{
		FreeLibrary(gameCode->gameCodeDLL);
		gameCode->gameCodeDLL = 0;
	}

	gameCode->isValid = false;
	gameCode->UpdateAndRender = 0;
	gameCode->Initialize = 0;
	//gameCode->GetSoundSamples = 0;
}


internal WindowDimension Win32GetWindowDimension()
{
	WindowDimension result;

	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	result.height = clientRect.bottom - clientRect.top;
	result.width = clientRect.right - clientRect.left;

	return result;
}


internal void Win32ProcessPendingMessages(GameInput* input)
{
	MSG msg;
	while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE))
	{
		switch (msg.message)
		{
			case WM_QUIT:
			{
				running = false;
			} break;

#if ATOMOSZ_NOTIMPLEMNTEDYET
			case WM_INPUT:
			{

	/** TODO: I can't remember how the logic goes (this was originally written in 2016),
				but I think when joysticks are registered they determine if they use
				RawInput or XInput (or DirectInput??). Will need to review, but I believe
				XInput is ONLY for XBox controllers, DirectInput is for controllers with
				forced feedback, and RawInput is for everything else. */

	/** If not using joysticks this can be removed  */
				PRAWINPUT pRawInput;
				UINT bufferSize;
				HANDLE hHeap;

				GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, NULL, &bufferSize, sizeof(RAWINPUTHEADER));
				hHeap = GetProcessHeap();
				pRawInput = (PRAWINPUT)HeapAlloc(hHeap, 0, bufferSize);
				if (!pRawInput)
					break;

				GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT,
					pRawInput, &bufferSize, sizeof(RAWINPUTHEADER));
				if (pRawInput->header.dwType == RIM_TYPEHID)
					gameEngine.parseRawInput(pRawInput);

				HeapFree(hHeap, 0, pRawInput);
				/** Joystick end */

			} break;
#endif

			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
			case WM_KEYDOWN:
			case WM_KEYUP:
			{
				uint8 vkCode = (uint8)msg.wParam;
				bool32 wasDown = ((msg.lParam & (1 << 30)) != 0);
				bool32 isDown = ((msg.lParam & (1 << 31)) == 0);
				/*if (isDown == wasDown) // couldn't this create problems if window loses focus??
					break;*/

				if (wasDown)
				{
					if (isDown)
						input->keys.keys[vkCode] = ButtonState::HELD;
					else // !isDown
						input->keys.keys[vkCode] = ButtonState::RELEASED;
				}
				else // !wasDown
				{
					if (isDown)
						input->keys.keys[vkCode] = ButtonState::PRESSED;
					else // !isDown
						input->keys.keys[vkCode] = ButtonState::UP;
				}

				if (isDown)
				{
					bool32 altKeyWasDown = (msg.lParam & (1 << 29));
					if ((vkCode == VK_F4) && altKeyWasDown)
						running = false;
					if ((vkCode == VK_RETURN) && altKeyWasDown)
						ToggleFullScreen();
				}
			} break;


			case WM_MOUSEMOVE:
			{
				int32 xPos = GET_X_LPARAM(msg.lParam);
				int32 yPos = GET_Y_LPARAM(msg.lParam);

				int32 yDiff = (msg.lParam >> 16) & 0xFFFF;

				Assert(yPos == yDiff);
				input->mouse.state.x = (float)xPos; // conversion required??
				input->mouse.state.y = (float)yPos;
				input->mouse.hasMoved = true;
			} break;

			/*
			case WM_LBUTTONDBLCLK:
			case WM_MBUTTONDBLCLK:
			case WM_RBUTTONDBLCLK:
			case WM_XBUTTONDBLCLK:
			case WM_MOUSEHOVER:
			case WM_MOUSELEAVE: // requires TrackMouseEvent to be called prior and
								// TrackMouseEvent must be called again when the mouse
								// re-enters the window.
			case WM_NCMOUSEMOVE: // when mouse moves outside of window IF another
								//window hasn't already captured the mouse.
			*/
			case WM_LBUTTONDOWN:
			{
				// @NOTE: if mouse leaves the window and click state changes,
				//		that click event is gone. We COULD track when the mouse
				//		leaves the window, then reset all clicks, or poll the
				//		current state of the buttons when the mouse returns.

				//Assert(!input->mouse.state.leftButton); 
				input->mouse.state.leftButton = true;
			} break;
			case WM_LBUTTONUP:
			{
				//Assert(input->mouse.state.leftButton);
				input->mouse.state.leftButton = false;
			} break;
			case WM_RBUTTONDOWN:
			{
				//Assert(!input->mouse.state.rightButton);
				input->mouse.state.rightButton = true;
			} break;
			case WM_RBUTTONUP:
			{
				//Assert(input->mouse.state.rightButton);
				input->mouse.state.rightButton = false;
			} break;
			case WM_MBUTTONDOWN:
			{
				//Assert(!input->mouse.state.middleButton);
				input->mouse.state.middleButton = true;
			} break;
			case WM_MBUTTONUP:
			{
				//Assert(input->mouse.state.middleButton);
				input->mouse.state.middleButton = false;
			} break;

			case WM_XBUTTONDOWN:
			{
				switch (GET_XBUTTON_WPARAM(msg.wParam))
				{
					case XBUTTON1:
					{
						input->mouse.state.xButton1 = true;
					} break;

					case XBUTTON2:
					{
						input->mouse.state.xButton2 = true;
					} break;
				}
			} break;

			case WM_XBUTTONUP:
			{
				switch (GET_XBUTTON_WPARAM(msg.wParam))
				{
					case XBUTTON1:
					{
						input->mouse.state.xButton1 = false;
					} break;

					case XBUTTON2:
					{
						input->mouse.state.xButton2 = false;
					} break;
				}
			} break;

			case WM_MOUSEWHEEL:
			{
				input->mouse.state.scrollWheelValue += GET_WHEEL_DELTA_WPARAM(msg.wParam);
			} break;

			case WM_KILLFOCUS:
			{
				int a = 0;
			} break;

			default:
			{
				TranslateMessage(&msg);
				DispatchMessageA(&msg);
			} break;
		}
	}
}

inline ButtonState::State SetButtonState(bool32 prevState, bool32 currentState)
{
	ButtonState::State result;
	if (prevState) // button was down
	{
		if (currentState) // still down
			result = ButtonState::HELD;
		else // now up
			result = ButtonState::RELEASED;
	}
	else // button was up
	{
		if (currentState) // but now is down
			result = ButtonState::PRESSED;
		else // still up
			result = ButtonState::UP;
	}

	return result;
}

inline void UpdateMouseState(MouseInput prevState, MouseInput* currentState)
{
	currentState->leftButton = SetButtonState(prevState.state.leftButton, currentState->state.leftButton);
	currentState->middleButton = SetButtonState(prevState.state.middleButton, currentState->state.middleButton);
	currentState->rightButton = SetButtonState(prevState.state.rightButton, currentState->state.rightButton);
	currentState->xButton1 = SetButtonState(prevState.state.xButton1, currentState->state.xButton1);
	currentState->xButton2 = SetButtonState(prevState.state.xButton2, currentState->state.xButton2);
}

internal LRESULT CALLBACK WndProc(HWND hWND, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	switch (msg)
	{
		case WM_CLOSE:
		{
			// @TODO: handle this with a message to user?
			running = false;
		}break;

		case WM_SETCURSOR:
		{
			result = DefWindowProcA(hWND, msg, wParam, lParam);
			/*if (debugShowCursor)
			{
				result = DefWindowProcA(hWND, msg, wParam, lParam);
			}
			else
				SetCursor(0); // hide cursor */
		}break;


		case WM_ACTIVATEAPP:
		{
			/* Set the opacity and transparency color key of the layered window. */
			if (wParam)
			{	// window gained focus
				//SetLayeredWindowAttributes(hWND, RGB(0, 0, 0), 255, LWA_ALPHA);
			}
			else
			{	// Window lost focus

				//SetLayeredWindowAttributes(hWND, RGB(0, 0, 0), 64, LWA_ALPHA);

			}
		} break;

		case WM_CREATE:
		{
#if ATOMOSZ_INTERNAL
			//getInputDeviceInfo(true, L"Devices.txt");
#endif

			RAWINPUTDEVICE rid[1];
			rid[0].usUsagePage = 1;
			rid[0].usUsage = 4; // joystick
			rid[0].dwFlags = 0;
			rid[0].hwndTarget = hWND;

			if (!RegisterRawInputDevices(rid, 1, sizeof(RAWINPUTDEVICE)))
			{
				Assert(!"On no!! Can't find Raw Input Device??");
				break;
			}

			RegisterControllers();
		} break;

		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			Assert(!"Keyboard input came in through a non-dispatch message!");
		} break;

		case WM_DEVICECHANGE:
		{
			PDEV_BROADCAST_HDR pDev = (PDEV_BROADCAST_HDR)(lParam);

			if (!pDev)
				break;

			if (pDev->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
			{
				PDEV_BROADCAST_DEVICEINTERFACE deviceInterface
					= (PDEV_BROADCAST_DEVICEINTERFACE)(pDev);
				switch (wParam)
				{
					case DBT_DEVICEARRIVAL:
					{
#if ATOMOSZ_NOTIMPLEMNTEDYET
						if (deviceInterface->dbcc_classguid == KSCATEGORY_AUDIO)
						{
							OutputDebugString("Audio interface added!\n");

							if (gameEngine.gameInitialized)
								gameEngine.onAudioDeviceChange();

							break;
						}
#endif

						if (deviceInterface->dbcc_classguid == guidHid)
						{
							OutputDebugString("Found a controller!\n");
							RegisterControllers();
							break;
						}
					} break;

					case DBT_DEVICEREMOVECOMPLETE:
					{
#if ATOMOSZ_NOTIMPLEMNTEDYET
						if (deviceInterface->dbcc_classguid == KSCATEGORY_AUDIO)
						{

							OutputDebugString("Audio interface removed!\n");
							break;
						}
#endif
						if (deviceInterface->dbcc_classguid == guidHid)
						{
// remove joysticks
							RegisterControllers();
							break;
						}
					} break;

				}

			}
		} break;

#if ATOMOSZ_NOTIMPLEMNTEDYET
		case WM_NCLBUTTONDOWN:
		{

			gameEngine.suspend();

		} break;

		case WM_KILLFOCUS:
		{
			//OutputDebugString(L"Lost Focus\n");
			gameEngine.suspend();
		} break;

		case WM_ACTIVATE:
		{

			//OutputDebugString(L"Got Focus\n");
			gameEngine.resume();
		} break;
#endif

		case WM_DESTROY:	// top right x button pressed or DestroyWindow(HWND) called
		{
			running = false;
		} break;

		default:
		{
			result = DefWindowProcA(hWND, msg, wParam, lParam);
		} break;
	}

	return result;
}


inline void MessageLoop(HINSTANCE hInstance)
{
	Win32State win32State = {};

	Win32GetEXEFilename(&win32State);

	char sourceGameCodeDLLFullpath[WIN32_STATE_FILE_NAME_COUNT];
	Win32BuildEXEPathFilename(&win32State, "AtomosZEngine.dll",
		sizeof(sourceGameCodeDLLFullpath), sourceGameCodeDLLFullpath);

	char tempGameCodeDLLFullpath[WIN32_STATE_FILE_NAME_COUNT];
	Win32BuildEXEPathFilename(&win32State, "AtomosZEngine_temp.dll",
		sizeof(tempGameCodeDLLFullpath), tempGameCodeDLLFullpath);

	char gameCodeLockFullpath[WIN32_STATE_FILE_NAME_COUNT];
	Win32BuildEXEPathFilename(&win32State, "lock.tmp",
		sizeof(gameCodeLockFullpath), gameCodeLockFullpath);


#if HANDMADE_INTERNAL
	LPVOID baseAddress = (LPVOID)Terabytes((uint64)2);
#else
	LPVOID baseAddress = 0;
#endif

	GameMemory gameMemory = {};
	gameMemory.permanentStorageSize = Megabytes(256);
	gameMemory.transientStorageSize = Gigabytes(1);

	gameMemory.DebugPlatformReadEntireFile = *DebugPlatformReadEntireFile;
	gameMemory.DebugPlatformFreeFileMemory = *DebugPlatformFreeFileMemory;
	gameMemory.DebugPlatformWriteEntireFile = *DebugPlatformWriteEntireFile;


	win32State.totalSize = gameMemory.permanentStorageSize + gameMemory.transientStorageSize;

	win32State.gameMemoryBlock = VirtualAlloc(baseAddress, (size_t)win32State.totalSize,
		MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	gameMemory.permanentStorage = win32State.gameMemoryBlock;
	gameMemory.transientStorage = ((uint8*)gameMemory.permanentStorage
		+ gameMemory.permanentStorageSize);

	for (int i = 0; i < ArrayCount(win32State.replayBuffers); ++i)
	{
		Win32ReplayBuffer* replayBuffer = &win32State.replayBuffers[i];
		replayBuffer->memoryBlock = VirtualAlloc(0, (size_t)win32State.totalSize,
			MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

		Assert(replayBuffer->memoryBlock);
		if (!replayBuffer->memoryBlock)
		{
			// @TODO: logging
		}
	}

	if (/*!samples ||*/ !gameMemory.permanentStorage || !gameMemory.transientStorage)
	{
		// Error logging
		Assert(!"Error creating memory");
		return;
	}


	GameInput gameInput = {};


	Win32GameCode gameCode = Win32LoadGameCode(
		sourceGameCodeDLLFullpath, tempGameCodeDLLFullpath, gameCodeLockFullpath);
	uint32 loadCounter = 0;


	if (!gameCode.Initialize)
	{
		Assert(!"Cannot find Initialize()!");
		return;
	}

	gameCode.Initialize(&gameMemory);
	InitializeSpriteBatch(&gameMemory, Win32GetWindowDimension(), hwnd);
	DirectXTextureData* testTexture = LoadTextureAsset(&gameMemory, "../assets/gfx/BigShipA.dds");
	Assert(testTexture);

#if ATOMOSZ_NOTIMPLEMNTEDYET
	if (!gameEngine.initEngine(hWND, hInstance, &gameInput))
	{
		GameEngine::errorMessage(L"Game Engine Initialization Failed", L"Error", true);
		ReleaseResources(0);
		return;
	}
#endif

	LARGE_INTEGER frequencyCount;
	QueryPerformanceCounter(&frequencyCount);
	LARGE_INTEGER lastCounter = Win32GetCurrentTime();
	programStart = lastCounter;
	uint64 lastCycleCount = __rdtsc(); // this is only useful for profiling


	running = true;
	while (running)
	{
		gameInput.deltaTime = targetSecondsPerFrame;

		FILETIME newDLLWriteTime = Win32GetLastWriteTime(sourceGameCodeDLLFullpath);
		if (CompareFileTime(&newDLLWriteTime, &gameCode.dllLastWriteTime))
		{
			Win32UnloadGameCode(&gameCode);
			gameCode = Win32LoadGameCode(
				sourceGameCodeDLLFullpath, tempGameCodeDLLFullpath, gameCodeLockFullpath);
			loadCounter = 0;
		}

		// controller stuff here
		GameInput prevState = gameInput;
		Win32ProcessPendingMessages(&gameInput);
		UpdateMouseState(prevState.mouse, &gameInput.mouse);


		ThreadContext thread = {};

		if (gameCode.UpdateAndRender)
			gameCode.UpdateAndRender(&thread, &gameMemory, &gameInput);

		DrawBatch batch = {};
		batch.position = {100, 250};
		batch.resource = testTexture->resource;
		batch.texture = testTexture->texture;
		uint32 width, height;
		GetTextureDimensions(batch.resource, &width, &height);
		batch.sourceRect = {0, 0, (long)width, (long)height};
		batch.scale = {1, 1};
		batch.origin = {(float)width / 2, (float)height / 2};
		batch.layerDepth = 0.1f;
		batch.rotation = 0.0f;
		batch.tint = DirectX::Colors::White;
		PushBatch(&gameMemory, batch);
		DirectXTKRender(&gameMemory);

		// Sound stuff here
#if ATOMOSZ_NOTIMPLEMNTEDYET
		if (gameCode.GetSoundSamples)
			gameCode.GetSoundSamples(&thread, &gameMemory, &soundBuffer);
#endif

		LARGE_INTEGER workCounter = Win32GetCurrentTime();
		float workSecondsElapsed = Win32GetSecondsElapsed(lastCounter, workCounter);
		float secondsElapsedForFrame = workSecondsElapsed;



		if (secondsElapsedForFrame >= targetSecondsPerFrame)
		{	// missed frame rate!
			char mbuffer[256];
			_snprintf_s(mbuffer, sizeof(mbuffer),
				"Missed frame rate! secondsElapsedForFrame: %.08f  targetSecondsPerFrame: %.08f\n",
				secondsElapsedForFrame, targetSecondsPerFrame);
			OutputDebugStringA(mbuffer);
		}
		else
		{
			if (sleepIsGranular)
			{
				DWORD sleepMS = (DWORD)(950.0f * (targetSecondsPerFrame - secondsElapsedForFrame));

				if (sleepMS > 0)
					Sleep(sleepMS - 1); // subtracting one seems to prevent from over sleeping

				/*LARGE_INTEGER afterSleepTime = Win32GetCurrentTime();
				float timeSlept = Win32GetSecondsElapsed(lastCounter, afterSleepTime);

				char mbuffer[256];
				_snprintf_s(mbuffer, sizeof(mbuffer),
					"sleepMS: %d\ttimeSlept: %.08f\n",
					sleepMS, timeSlept);
				OutputDebugStringA(mbuffer);*/
			}

			float testSecondsElapsedForFrame = Win32GetSecondsElapsed(lastCounter, Win32GetCurrentTime());

			if (testSecondsElapsedForFrame > targetSecondsPerFrame)
			{ // slept to long! Missed the frame rate!
				char mbuffer[256];
				_snprintf_s(mbuffer, sizeof(mbuffer),
					"Over slept!!!\nsecondsElapsedForFrame: %.08f testSecondsElapsedForFrame: %.08f  targetSecondsPerFrame: %.08f\n",
					secondsElapsedForFrame, testSecondsElapsedForFrame, targetSecondsPerFrame);
				OutputDebugStringA(mbuffer);
			}

			float sleepEpsilon = 0.0000003f;
			while (secondsElapsedForFrame < targetSecondsPerFrame - sleepEpsilon)
			{	// run out the clock until we hit our target frame time.
				secondsElapsedForFrame = Win32GetSecondsElapsed(lastCounter, Win32GetCurrentTime());
			}
		}

		if (secondsElapsedForFrame > targetSecondsPerFrame)
		{ // slept to long! Missed the frame rate!
			char mbuffer[256];
			_snprintf_s(mbuffer, sizeof(mbuffer),
				"Over slept!\tsecondsElapsedForFrame: %.08f\ttargetSecondsPerFrame: %.08f\n",
				secondsElapsedForFrame, targetSecondsPerFrame);
			OutputDebugStringA(mbuffer);
		}


		LARGE_INTEGER endCounter = Win32GetCurrentTime();
		float msPerFrame = 1000.0f * Win32GetSecondsElapsed(lastCounter, endCounter);
		lastCounter = endCounter;

#if 0
		uint64 endCycleCount = __rdtsc();
		uint64 cyclesElapsed = endCycleCount - lastCycleCount;
		lastCycleCount = endCycleCount;

		double fps = 0.0;
		double mCyclesPerFrame = ((float)cyclesElapsed / (1000 * 1000));

		char fpsBuffer[256];
		_snprintf_s(fpsBuffer, sizeof(fpsBuffer),
			"FrameTime: %.02fms | FPS: %.02f | %.02f megacycles/frame\n",
			msPerFrame, fps, mCyclesPerFrame);
		OutputDebugStringA(fpsBuffer);
#endif
	}
}


inline bool InitWindow(HINSTANCE hInstance, int showWnd)
{
	//WNDCLASSEX wc;
	WNDCLASSA wc = {}; // Casey used this one

	//wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;			// mo' styles http://msdn.microsoft.com/en-us/library/ms633574(VS.85).aspx#class_styles
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;					// instance of current app
	//wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);	// title bar icon
	wc.hCursor = LoadCursor(0, IDC_ARROW);		// mo' mouse cursors http://msdn.microsoft.com/en-us/library/ms648391(VS.85).aspx
	//wc.hbrBackground = 0/*(HBRUSH) (COLOR_WINDOW)*/;
	wc.lpszClassName = "TenderTorrentWindowClass";
	//wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);	// taskbar icon

	//if (!RegisterClassEx(&wc))
	if (!RegisterClassA(&wc))
	{
		MessageBox(NULL, "Error registering class", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	int posX, posY;
	posX = posY = 0;
	int windowX, windowY;
	windowX = windowY = 0;

	RECT winRect = {0, 0, (int32)initialWindowSize.width, (int32)initialWindowSize.height};
	//AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);
	windowX = winRect.right - winRect.left;
	windowY = winRect.bottom - winRect.top;
	// If windowed then set it to global default resolution
	// and place the window in the middle of the screen.
	posX = (GetSystemMetrics(SM_CXSCREEN) - initialWindowSize.width) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - initialWindowSize.height) / 2;


	//DWORD windowStyle = (WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME | /*WS_CAPTION | WS_SYSMENU | */
	//	WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
	DWORD windowStyle = (WS_OVERLAPPEDWINDOW | WS_VISIBLE);/* Casey used this */

	//hwnd = CreateWindowEx(
	hwnd = CreateWindowExA( // Casey used this one
		0,						// extended style, check em out here https://msdn.microsoft.com/en-us/library/61fe4bte(v=vs.140).aspx
		wc.lpszClassName,
		"Tender Torrent <3",	// title bar text
		windowStyle,			// window style, mo' styles http://msdn.microsoft.com/zh-cn/library/czada357.aspx
		posX, posY,				// top left of window
		windowX, windowY,		// window dimensions
		0,						// handle to parent window
		0,						// handle to a menu
		hInstance,				// instance of current program
		0);						// used for MDI client window

	if (!hwnd)
	{
		MessageBox(NULL, "Error creating window", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	// Makes window transparent. Interesting! But useless?
	/*SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_LAYERED);
	SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 100, LWA_ALPHA);*/

	//ShowWindow(hwnd, showWnd);
	//UpdateWindow(hwnd);


	return true;
}


/** Main windows function.
@nShowWnd how window should be displayed. Examples: SW_SHOWMAXIMIZED, SW_SHOW, SW_SHOWMINIMIZED. */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	guidHid = GUID();
	guidHid.Data1 = 0x4D1E55B2L;
	guidHid.Data2 = 0xF16F;
	guidHid.Data3 = 0x11CF;
	guidHid.Data4[0] = 0x88;
	guidHid.Data4[1] = 0xCB;
	guidHid.Data4[2] = 0x00;
	guidHid.Data4[3] = 0x11;
	guidHid.Data4[4] = 0x11;
	guidHid.Data4[5] = 0x00;
	guidHid.Data4[6] = 0x00;
	guidHid.Data4[7] = 0x30;

	LARGE_INTEGER frequencyCount;
	/* Retrieves the frequency of the performance counter. The frequency of the
		performance counter is fixed at system boot and is consistent across all processors. */
	QueryPerformanceFrequency(&frequencyCount);
	perfCountFrequency = frequencyCount.QuadPart;
	// Set the windows scheduler granularity to 1 ms
	// so that our Sleep() can be more granular.
	UINT desiredSchedulerMS = 1;
	sleepIsGranular = (timeBeginPeriod(desiredSchedulerMS) == TIMERR_NOERROR);

	if (!InitWindow(hInstance, nShowCmd))
	{
		MessageBox(0, "Window Initialization - Failed", "Error", MB_OK);
		ReleaseResources(0);
		return 0;
	}

	int monitorRefreshHz = 60;
	HDC refreshDC = GetDC(hwnd);
	int win32RefreshRate = GetDeviceCaps(refreshDC, VREFRESH);
	ReleaseDC(hwnd, refreshDC);
	if (win32RefreshRate > 1)
		monitorRefreshHz = win32RefreshRate;

	float gameUpdateHz = (float)monitorRefreshHz /*/ 2.0f*/;
	targetSecondsPerFrame = 1.0f / (float)gameUpdateHz;

	// Set up sound buffer here




	// Set up memory arena here






	// listen for new devices
	DEV_BROADCAST_DEVICEINTERFACE notificationFilter;
	ZeroMemory(&notificationFilter, sizeof(notificationFilter));
	notificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	notificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	HDEVNOTIFY newInterface = RegisterDeviceNotification(hwnd, &notificationFilter,
		DEVICE_NOTIFY_ALL_INTERFACE_CLASSES);


	MessageLoop(hInstance); /* Main program loop */
	ReleaseResources(newInterface);


	return 0;
}





/** If using joysticks this is required. */
int RegisterControllers()
{
#if ATOMOSZ_NOTIMPLEMNTEDYET
	UINT nDevices;
	PRAWINPUTDEVICELIST pRawInputDeviceList;
	if (GetRawInputDeviceList(NULL, &nDevices, sizeof(RAWINPUTDEVICELIST)) != 0)
	{
		Assert(!"On no!! Can't find Raw Input Device??");
		return -1;
	}

	if ((pRawInputDeviceList = (PRAWINPUTDEVICELIST)malloc(sizeof(RAWINPUTDEVICELIST) * nDevices)) == NULL)
	{
		Assert(!"Initialization failed...");
		return -1;
	}

	int nNoOfDevices = 0;
	if ((nNoOfDevices = GetRawInputDeviceList(
		pRawInputDeviceList, &nDevices, sizeof(RAWINPUTDEVICELIST))) == ((UINT)-1))
	{
		Assert(!"No devices found");
		return -1;
	}

	RID_DEVICE_INFO rdi;
	rdi.cbSize = sizeof(RID_DEVICE_INFO);

	vector<ControllerDevice> controllerDevices;
	for (int i = 0; i < nNoOfDevices; i++)
	{
		UINT size = 256;
		TCHAR tBuffer[256] = {0};

		if (GetRawInputDeviceInfo(pRawInputDeviceList[i].hDevice,
			RIDI_DEVICENAME, tBuffer, &size) < 0)
		{
// Error in reading device name
			continue;
		}

		UINT cbSize = rdi.cbSize;
		if (GetRawInputDeviceInfo(pRawInputDeviceList[i].hDevice,
			RIDI_DEVICEINFO, &rdi, &cbSize) < 0)
		{
// Error in reading information
			wostringstream wss;
			wss << L"Device Name: " << tBuffer << "\n";
			wss << "Error reading information" << std::endl;
			Assert(!wss.str().c_str());
			continue;
		}


		if (rdi.dwType == RIM_TYPEHID)
		{
			if (rdi.hid.usUsagePage == 1)
			{
				if (rdi.hid.usUsage == 4)
				{
					ControllerDevice cd;
					cd.handle = pRawInputDeviceList[i].hDevice;
					cd.isRawInput = true;
					controllerDevices.push_back(cd);
				}
				else if (rdi.hid.usUsage == 5)
				{
					ControllerDevice cd;
					cd.handle = pRawInputDeviceList[i].hDevice;
					cd.isRawInput = false;
					controllerDevices.push_back(cd);
				}

			}
		}
	}

	gameEngine.addJoysticks(controllerDevices);


	free(pRawInputDeviceList);
#endif
	return 1;
}

#if 0
/* Finds and list all HID devices. For device finding, debugging, etc.
	@TODO: The output streams used here here requires sstream.h, which seems to clash
	with stdio.h. */
int GetInputDeviceInfo(bool writeToFile, std::wstring filename)
{
	UINT nDevices;
	PRAWINPUTDEVICELIST pRawInputDeviceList;
	if (GetRawInputDeviceList(NULL, &nDevices, sizeof(RAWINPUTDEVICELIST)) != 0)
	{
		Assert(!"On no!! Can't find Raw Input Device??");
		return -1;
	}

	if ((pRawInputDeviceList = (PRAWINPUTDEVICELIST)malloc(sizeof(RAWINPUTDEVICELIST) * nDevices)) == NULL)
	{
		Assert(!"Initialization failed...");
		return -1;
	}

	int nNoOfDevices = 0;
	if ((nNoOfDevices = GetRawInputDeviceList(pRawInputDeviceList, &nDevices, sizeof(RAWINPUTDEVICELIST))) == ((UINT)-1))
	{
		Assert(!"No devices found");
		return -1;
	}

	RID_DEVICE_INFO rdi;
	rdi.cbSize = sizeof(RID_DEVICE_INFO);
	std::wostringstream wss;
	wss << "Number of devices: " << nNoOfDevices << std::endl << "***********************" << std::endl;
	for (int i = 0; i < nNoOfDevices; i++)
	{
		UINT size = 256;
		TCHAR tBuffer[256] = {0};

		if (GetRawInputDeviceInfo(pRawInputDeviceList[i].hDevice, RIDI_DEVICENAME, tBuffer, &size) < 0)
		{
// Error in reading device name
			wss << "Error reading device name" << std::endl;
			//continue;
		}


		wss << L"Device Name: " << tBuffer << "\n";
		UINT cbSize = rdi.cbSize;
		if (GetRawInputDeviceInfo(pRawInputDeviceList[i].hDevice, RIDI_DEVICEINFO, &rdi, &cbSize) < 0)
		{
// Error in reading information
			wss << "Error reading information" << std::endl;
		}

		if (rdi.dwType == RIM_TYPEMOUSE)
		{
			wss << "ID for Mouse:" << rdi.mouse.dwId << std::endl;
			wss << "Number of Buttons:" << rdi.mouse.dwNumberOfButtons << std::endl;
			wss << "Sample rate(Number of data points):" << rdi.mouse.dwSampleRate << std::endl;
		}
		else if (rdi.dwType == RIM_TYPEKEYBOARD)
		{
			wss << "Keyboard Mode:" << rdi.keyboard.dwKeyboardMode << std::endl;
			wss << "Number of function keys:" << rdi.keyboard.dwNumberOfFunctionKeys << std::endl;
			wss << "Number of indicators:" << rdi.keyboard.dwNumberOfIndicators << std::endl;
			wss << "Number of keys total: " << rdi.keyboard.dwNumberOfKeysTotal << std::endl;
			wss << "Type of the keyboard: " << rdi.keyboard.dwType << std::endl;
			wss << "Subtype of the keyboard: " << rdi.keyboard.dwSubType << std::endl;
		}
		else if (rdi.dwType == RIM_TYPEHID)
		{
			wss << "Vendor Id:" << rdi.hid.dwVendorId << std::endl;
			wss << "Product Id:" << rdi.hid.dwProductId << std::endl;
			wss << "Version No:" << rdi.hid.dwVersionNumber << std::endl;
			wss << "Usage for the device: " << rdi.hid.usUsage << std::endl;
			wss << "Usage Page for the device: " << rdi.hid.usUsagePage << std::endl;


		}
		wss << "***********************" << std::endl << std::endl;
	}

	if (writeToFile)
	{
		std::wofstream file;
		file = std::wofstream(filename);
		file << wss.str();
		file.close();

	}
	else
		OutputDebugStringW(wss.str().c_str());

	free(pRawInputDeviceList);
	return 0;
}
#endif

