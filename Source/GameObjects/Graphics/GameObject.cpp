#include "../../pch.h"
#include "GameObject.h"

#include "../../Engine/GameEngine.h"


GameObject::GameObject() : Sprite() {
}


GameObject::~GameObject() {
}

float GameObject::getFloatFrom(xml_node node) {

	string value = node.text().as_string();
	if (isdigit(value[0]))
		return node.text().as_float();
	return getFloatFrom(value);
}

float GameObject::getFloatFrom(xml_attribute attr) {

	string value = attr.as_string();
	if (isdigit(value[0]))
		return attr.as_float();
	return getFloatFrom(value);
}

float GameObject::getFloatFrom(string value) {
	size_t i = value.find_first_of(" ");
	if (i == std::string::npos) {
		// ohnos dudes

	}
	string token = value.substr(0, i);


	float num = 0;
	if (token.compare("WINDOW_WIDTH") == 0) {
		num = (float) camera.viewportWidth;
	} else if (token.compare("WINDOW_HEIGHT") == 0) {
		num = (float) camera.viewportHeight;
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
