#pragma once

#include <tchar.h>
#include <sstream>
#include <strsafe.h>

#include "../GameObjects/Graphics/FontSet.h"

using namespace std;


class TextLabel {
public:
	TextLabel(Vector2 position, FontSet* font);
	~TextLabel();


	void draw(SpriteBatch* batch);

	wstring label;


private:

	FontSet* font;
	Vector2 position;
};

