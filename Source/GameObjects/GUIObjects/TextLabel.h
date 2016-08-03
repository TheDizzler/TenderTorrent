#pragma once

#include <tchar.h>
#include <sstream>
#include <strsafe.h>

#include "../Graphics/FontSet.h"

using namespace std;


class TextLabel {
public:
	TextLabel(Vector2 position, FontSet* font);
	~TextLabel();

	void draw(SpriteBatch* batch);

	void setText(string text);
	void setText(wostringstream& text);
	void setText(wstring text);

	const wchar_t* getText();
	
	Vector2 position;

private:

	wstring label;
	FontSet* font;
	
};

