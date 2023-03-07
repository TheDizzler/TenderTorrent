#if !defined(STRING_HELPER_H)
/*****************************************************************************
*	$Note:	$
*	$Author: TDGillon $
*	$Notice: (C) Copyright 2023 by AtomosZ Industries. All rights reserved. $
*****************************************************************************/

inline int32 StringLength(char* text)
{
	int32 i = 0;
	while (*text++)
		++i;
	return i;
}

inline int32 StringLength(WCHAR* text)
{
	int32 i = 0;
	while (*text++)
		++i;
	return i;
}


/// <summary>
/// @TODO: there is an icky "new" in here.
/// </summary>
/// <param name="text"></param>
/// <returns></returns>
inline wchar_t* ConvertCharStarToWCharT(char* text)
{
	size_t cSize = StringLength(text) + 1;
	wchar_t* wc = new wchar_t[cSize];
	size_t outsize;
	mbstowcs_s(&outsize, wc, cSize, text, cSize - 1);

	return wc;
}


internal void CatStrings(size_t sourceACount, char* sourceA,
	size_t sourceBCount, char* sourceB, size_t destCount, char* dest)
{
	for (int i = 0; i < sourceACount; ++i)
		*dest++ = *sourceA++;

	for (int i = 0; i < sourceBCount; ++i)
		*dest++ = *sourceB++;

	*dest++ = 0;
}

internal void CatStrings(size_t sourceACount, WCHAR* sourceA,
	size_t sourceBCount, WCHAR* sourceB, size_t destCount, WCHAR* dest)
{
	for (int i = 0; i < sourceACount; ++i)
		*dest++ = *sourceA++;

	for (int i = 0; i < sourceBCount; ++i)
		*dest++ = *sourceB++;

	*dest++ = 0;
}

#define STRING_HELPER_H
#endif