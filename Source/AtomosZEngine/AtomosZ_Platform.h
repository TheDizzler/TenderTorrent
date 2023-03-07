#if !defined(ATOMOSZ_PLATFORM_H)
/*****************************************************************************
*	$Note:	$
*	$Author: TDGillon $
*	$Notice: (C) Copyright 2023 by AtomosZ Industries. All rights reserved. $
*****************************************************************************/


#include <stdint.h>
#include <stddef.h>


#if ATOMOSZ_DEBUG
#define Assert(expression) if (!(expression)) {*(int*)0 = 0;}
#else
#define Assert(expression)
#endif

#define internal static
#define local static
#define global static

internal void ToggleFullScreen();
#define InvalidCodePath Assert(!"InvalidCodePath");
#define FatalError if (IsFullScreen()) { ToggleFullScreen();} *(int*)0 = 0;

#define Kilobytes(value) ((value) * 1024LL)
#define Megabytes(value) (Kilobytes(value) * 1024LL)
#define Gigabytes(value) (Megabytes(value) * 1024LL)
#define Terabytes(value) (Gigabytes(value) * 1024LL)

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))


typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef int32 bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float real32;
typedef double real64;

typedef size_t memory_index;



#include "UserInput.h"


/* Assert file is only 32 bits (less than 4GBs) and then truncate. */
inline uint32 SafeTruncateUInt64(uint64 value)
{
	Assert(value <= 0xFFFFFFFF);
	return (uint32)value;
}

struct ThreadContext
{
	int placeHolder;
};

// Services that the platform layer provides to the game


	/* @IMPORTANT:
		These are NOT for doing anything in the shipping game - they are
		blocking and the write doesn't protect against lost data! */

struct DebugReadFileResult
{
	uint32 contentsSize;
	void* contents;
};

#define DEBUG_PLATFORM_FREE_FILE_MEMORY(name)\
	void name(ThreadContext* thread, void *memory)
typedef DEBUG_PLATFORM_FREE_FILE_MEMORY(Debug_Platform_Free_File_Memory);

#define DEBUG_PLATFORM_READ_ENTIRE_FILE(name)\
	DebugReadFileResult name(ThreadContext* thread, char *filename)
typedef DEBUG_PLATFORM_READ_ENTIRE_FILE(Debug_Platform_Read_Entire_File);

#define DEBUG_PLATFORM_WRITE_ENTIRE_FILE(name)\
	bool32 name(ThreadContext* thread, char* filename, uint32 memorySize, void* memory)
typedef DEBUG_PLATFORM_WRITE_ENTIRE_FILE(Debug_Platform_Write_Entire_File);


//  Services that the game provides to the platform layer.

struct GameMemory
{
	uint64 permanentStorageSize;
	/* REQUIRED to be cleared to zero at startup */
	void* permanentStorage;
	uint64 transientStorageSize;
	/* REQUIRED to be cleared to zero at startup */
	void* transientStorage;

	void* graphicsDevice;

	Debug_Platform_Read_Entire_File* DebugPlatformReadEntireFile;
	Debug_Platform_Free_File_Memory* DebugPlatformFreeFileMemory;
	Debug_Platform_Write_Entire_File* DebugPlatformWriteEntireFile;
};



#define GAME_UPDATE_AND_RENDER(name) void name(ThreadContext* thread,\
	GameMemory* memory,	GameInput* input)
typedef GAME_UPDATE_AND_RENDER(Game_Update_And_Render);

#define GAME_INITIALIZE(name) void name(GameMemory* memory)
typedef GAME_INITIALIZE(Game_Initialize);

//#define GAME_GET_SOUND_SAMPLES(name) void name(ThreadContext* thread,\
//	GameMemory* memory,	SoundOutputBuffer* soundBuffer)
//typedef GAME_GET_SOUND_SAMPLES(Game_Get_Sound_Samples);



#define DEBUG_HITBOXES 0
// TODO: This should be in a game def file.
#define MAX_PLAYERS 2

// DELET this
namespace Globals
{
	//extern int WINDOW_WIDTH;	// in pixels
	//extern int WINDOW_HEIGHT;	// in pixels

#if ATOMOSZ_NOTIMPLEMNTEDYET
	const static Vector2 targetResolution = Vector2(256, 224); // true SNES resolution
	const static Vector2 SHIP_STORE_POSITION = Vector2(-100, -100); // in world coords
	const static Vector2 BULLET_STORE_POSITION = Vector2(-100, -50); // in world coors
#endif

	/**	0 = Present as fast as possible.
		1 = Lock to screen refresh rate. */
	extern int vsync_enabled;


	/* For reference purposes only. */
	/*wostringstream ws;
	ws << "some writing here" << someVariable << "\n";
	OutputDebugString(ws.str().c_str());*/
};

#define ATOMOSZ_PLATFORM_H
#endif