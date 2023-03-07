#if !defined(WIN32_ATOMOSZ_H)
/*****************************************************************************
*	$Note:	$
*	$Author: TDGillon $
*	$Notice: (C) Copyright 2023 by AtomosZ Industries. All rights reserved. $
*****************************************************************************/

struct WindowDimension
{
	uint32 width;
	uint32 height;
};

struct Win32GameCode
{
	HMODULE gameCodeDLL;
	FILETIME dllLastWriteTime;

	// This can be null!
	//Game_Get_Sound_Samples* GetSoundSamples;
	// This can be null!
	Game_Update_And_Render* UpdateAndRender;
	Game_Initialize* Initialize;

	bool32 isValid;
};


#define WIN32_STATE_FILE_NAME_COUNT MAX_PATH
struct Win32ReplayBuffer
{
	char replayFileName[WIN32_STATE_FILE_NAME_COUNT];
	void* memoryBlock;
};

struct Win32State
{
	uint64 totalSize;
	void* gameMemoryBlock;
	Win32ReplayBuffer replayBuffers[4];

	HANDLE recordingHandle;
	int inputRecordingIndex;
	HANDLE playbackHandle;
	int inputPlaybackIndex;


	char exeFileName[WIN32_STATE_FILE_NAME_COUNT];
	char* onePastLastExeFileNameSlash;
};

#define WIN32_ATOMOSZ_H
#endif