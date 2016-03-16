#pragma once


/* Global variables and includes */
#include <comdef.h>


namespace Globals {

	const int WINDOW_WIDTH = 800;	// in pixels
	const int WINDOW_HEIGHT = 600;	// in pixels


	inline bool reportError(HRESULT hr) {

		if (FAILED(hr)) {
			_com_error err(hr);
			MessageBox(NULL, err.ErrorMessage(), TEXT("This is SRS Error"), MB_OK | MB_ICONERROR);
			return true;
		}

		return false;
	}

};
