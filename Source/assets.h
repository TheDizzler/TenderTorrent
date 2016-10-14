#pragma once


/* Global variables and includes */
#include <wrl.h>
#include <comdef.h>
#include <algorithm> 
#include <cctype>

//#include "GameObjects\Graphics\Sprite.h"

using namespace Microsoft::WRL;


namespace Assets {

	const static char* levelManifestFile = "assets/LevelManifest.xml";

	/*const static wchar_t* arialFontFile = L"assets/Arial.spritefont";

	const static wchar_t* mouseIconFile = L"assets/GUI/mouse icon.dds";
	const static wchar_t* mouseReticleFile = L"assets/GUI/reticle (16x16).dds";
	const static wchar_t* buttonUpFile = L"assets/GUI/button up (256x64).dds";
	const static wchar_t* buttonDownFile = L"assets/GUI/button down (256x64).dds";*/

	//const static wchar_t* whitePixelFile = L"assets/GUI/WhitePixel.dds";

	/*const static wchar_t* comboButtonClosedFile
		= L"assets/GUI/ComboButtonClosed.dds";
	const static wchar_t* comboButtonPressedClosedFile
		= L"assets/GUI/ComboButtonPressedClosed.dds";
	const static wchar_t* comboButtonOpenFile = L"assets/GUI/ComboButtonOpen.dds";
	const static wchar_t* comboButtonPressedOpenFile
		= L"assets/GUI/ComboButtonPressedOpen.dds";
	const static wchar_t* scrollBarUpFile = L"assets/GUI/ScrollBarUp.dds";
	const static wchar_t* scrollBarUpPressedFile
		= L"assets/GUI/ScrollBarUpPressed.dds";
	const static wchar_t* scrollBarDownFile = L"assets/GUI/ScrollBarDown.dds";
	const static wchar_t* scrollBarDownPressedFile
		= L"assets/GUI/ScrollBarDownPressed.dds";*/

	/** This needs to be rejiggered. */
	//const static wchar_t* pauseOverlayFile = L"assets/GUI/pinkblock(16x16).dds";
	//const static wchar_t* uglyDialogBox = L"assets/GUI/dialog pink (200x200).dds";


	//const static wchar_t* rearAttackShipFile = L"assets/enemy ship A.dds";
	//const static wchar_t* starEnemyShip = L"assets/enemy ship B.dds";

	//const static wchar_t* enemyBulletA = L"assets/bullet yellow.dds";

	//const static wchar_t* cornerFrameFile = L"assets/bg/corner frame.dds";

	//const static wchar_t* levelMakoXML = L"assets/bg/mako.xml";

};
