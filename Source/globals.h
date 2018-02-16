#include "pch.h"
#pragma once

#define MAX_PLAYERS 2

namespace Globals {

	extern int WINDOW_WIDTH;	// in pixels
	extern int WINDOW_HEIGHT;	// in pixels

	const static Vector2 targetResolution = Vector2(256, 224); // true SNES resolution

	/**	0 = Present as fast as possible.
		1 = Lock to screen refresh rate. */
	extern int vsync_enabled;
	extern bool FULL_SCREEN;

	

	/* For reference purposes only. */
	/*wostringstream ws;
	ws << "some writing here" << someVariable << "\n";
	OutputDebugString(ws.str().c_str());*/

	

	inline float getFloatFrom(string value) {
		
		size_t i = value.find_first_of(" ");
		if (i == std::string::npos) {
			// ohnos dudes
			
		}
		string token = value.substr(0, i);


		float num = 0;
		if (token.compare("WINDOW_WIDTH") == 0) {
			num = (float) Globals::WINDOW_WIDTH;
		} else if (token.compare("WINDOW_HEIGHT") == 0) {
			num = (float) Globals::WINDOW_HEIGHT;
		}

		if (i == std::string::npos)
			return num;

		token = value.substr(i);
		i = token.find_first_not_of(" ");
		if (i == std::string::npos) {
			// ohnos dudes
		}

		token = token.substr(i);
		char op = token[0];
		if (op == 'W')
			return num;
		token = token.substr(1);
		i = token.find_first_not_of(" ");
		if (i == std::string::npos) {
			// ohnos dudes
		}
		token = token.substr(i);
		int next = stoi(token);
		if (op == '+') {
			num += (float) next;

		} else if (op == '-') {
			num -= (float) next;

		} else if (op == '/') {
			num /= next;

		} else if (op == '*') {
			num *= next;

		}

		/*wostringstream ws;
		ws << token.c_str() << "\n";
		OutputDebugString(ws.str().c_str());*/

		return num;

	}

	/* Extract int from node text. */
	inline float getFloatFrom(xml_node node) {

		string value = node.text().as_string();
		if (isdigit(value[0]))
			return node.text().as_float();
		return getFloatFrom(value);
	}

	/* Extract int from node attribute. */
	inline float getFloatFrom(xml_attribute attr) {

		string value = attr.as_string();
		if (isdigit(value[0]))
			return attr.as_float();
		return getFloatFrom(value);
	}

};
