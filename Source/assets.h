#pragma once


/* Global variables and includes */
#include <comdef.h>
#include <algorithm> 
#include <cctype>

namespace Assets {

	const static wchar_t* arialFontFile = L"assets/Arial.spritefont";

	const static wchar_t* mouseReticle = L"assets/reticle (16x16).dds";
	const static wchar_t* buttonUpFile = L"assets/button up (256x64).dds";
	const static wchar_t* buttonDownFile = L"assets/button down (256x64).dds";

	const static wchar_t* pauseOverlayFile = L"assets/pinkblock(16x16).dds";

	const static wchar_t* rearAttackShipFile = L"assets/enemy ship A.dds";
	const static wchar_t* starEnemyShip = L"assets/enemy ship B.dds";

	const static wchar_t* enemyBulletA = L"assets/bullet yellow.dds";

	const static wchar_t* cornerFrameFile = L"assets/bg/corner frame.dds";

	const static wchar_t* levelMakoXML = L"assets/bg/mako.xml";
	/*const static wchar_t* bgMakoBGFile = L"assets/bg/mako00.dds";
	const static wchar_t* bgMako01File = L"assets/bg/mako01";
	const static int numMako01Files = 9;
	const static Vector2 aSize(121, 98);
	const static Vector2 aPos(118, 347);
	const static Vector2 bSize(87, 90);
	const static Vector2 bPos(30, 449);
	const static Vector2 cSize(133, 243);
	const static Vector2 cPos(303, 360);
	const static Vector2 dSize(97, 116);
	const static Vector2 dPos(212, 610);
	const static Vector2 eSize(131, 136);
	const static Vector2 ePos(87, 535);
	const static Vector2 fSize(146, 37);
	const static Vector2 fPos(298, 714);
	const static Vector2 gSize(26, 25);
	const static Vector2 gPos(101, 438);
	const static Vector2 hSize(26, 26);
	const static Vector2 hPos(289, 594);
	const static Vector2 iSize(310, 138);
	const static Vector2 iPos(151, 837);*/

	inline wchar_t* convertCharStarToWCharT(const char* text) {

		const size_t cSize = strlen(text) + 1;
		wchar_t* wc = new wchar_t[cSize];
		mbstowcs(wc, text, cSize);

		return wc;
	}

	// trim from start (in place)
	static inline void ltrim(std::string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(),
			std::not1(std::ptr_fun<int, int>(std::isspace))));
	}

	// trim from end (in place)
	static inline void rtrim(std::string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(),
			std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	}

	// trim from both ends (in place)
	static inline void trim(std::string &s) {
		ltrim(s);
		rtrim(s);
	}

	static void getTextureDimensions(ID3D11Resource* res, UINT* width, UINT* height) {

		D3D11_RESOURCE_DIMENSION dim;
		res->GetType(&dim);

		switch (dim) {
			case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
			{

				auto texture = reinterpret_cast<ID3D11Texture2D*>(res);
				D3D11_TEXTURE2D_DESC desc;
				texture->GetDesc(&desc);
				if (width)
					*width = desc.Width; // width in pixels
				if (height)
					*height = desc.Height; // height in pixels

				break;

			}
			default:

				if (width)
					*width = 0; // width in pixels
				if (height)
					*height = 0; // height in pixels


		}
	}

};
