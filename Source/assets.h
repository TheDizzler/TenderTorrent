#pragma once


/* Global variables and includes */
#include <comdef.h>


namespace Assets {

	const static wchar_t* arialFontFile = L"assets/Arial.spritefont";

	const static wchar_t* buttonUpFile = L"assets/button up (256x64).dds";
	const static wchar_t* buttonDownFile = L"assets/button down (256x64).dds";

	
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
