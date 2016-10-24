#include "RearAttackShip.h"

#include "../globals.h"
inline int getInt(xml_attribute attr) {

	string value = attr.as_string();
	if (isdigit(value[0]))
		return attr.as_int();

	/*int i = -1;
	while (isalpha(value[++i])) {
	}*/
	int i = value.find_first_of(" ");
	string token = value.substr(0, i);


	int num = 0;
	if (token.compare("WINDOW_WIDTH") == 0) {
		num = Globals::WINDOW_WIDTH;

	} else if (token.compare("WINDOW_HEIGHT") == 0) {
		num = Globals::WINDOW_HEIGHT;

	}

	if (i == -1)
		return num;

	token = value.substr(i);
	i = token.find_first_not_of(" ");

	token = token.substr(i);
	char op = token[0];
	if (op == 'W')
		return num;
	token = token.substr(1);
	i = token.find_first_not_of(" ");
	token = token.substr(i);
	int next = stoi(token);
	if (op == '+') {
		num += next;

	} else if (op == '-') {
		num -= next;

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

RearAttackShip::RearAttackShip(xml_node mirrorNode) {

	rotation = XM_PI;
	xml_node startNode = mirrorNode.child("start");
	xml_node controlNode = mirrorNode.child("controlPoint");
	xml_node climaxNode = mirrorNode.child("climax");
	xml_node endNode = mirrorNode.child("end");
	startPos = Vector2(getInt(startNode.attribute("x")), getInt(startNode.attribute("y")));
	controlPoint = Vector2(getInt(controlNode.attribute("x")), getInt(controlNode.attribute("y")));
	climaxPos = Vector2(getInt(climaxNode.attribute("x")), getInt(climaxNode.attribute("y")));;
	endPos = Vector2(getInt(endNode.attribute("x")), getInt(endNode.attribute("y")));


	// changes these to read xml
	maxHealth = 6;


	position = startPos;
	weaponLocation = position;
	health = maxHealth;
}



RearAttackShip::RearAttackShip() {
}


RearAttackShip::RearAttackShip(bool isRght) : EnemyShip(position) {

	rotation = XM_PI;

	//rightSide = isRght;
	if (isRght) {
		startPos = startPosRightSide;
		climaxPos = Vector2(Globals::WINDOW_WIDTH - 125, 150);
		controlPoint = Vector2(Globals::WINDOW_WIDTH, 0);
	} else {
		startPos = startPosLeftSide;
		climaxPos = Vector2(125, 150);
		controlPoint = Vector2(0, 0);
	}
	endPos = Vector2(Globals::WINDOW_WIDTH / 2, Globals::WINDOW_HEIGHT + 120);
	maxHealth = 6;


	position = startPos;
	weaponLocation = position;
	health = maxHealth;
}

RearAttackShip::~RearAttackShip() {
}

void RearAttackShip::reset() {
	position = startPos;
	weaponLocation = position;
	health = maxHealth;
	timeAlive = 0;
	isAlive = true;
	fired = false;
}


double timeToClimax = 3.0;
double timeToFire = 2.5;
void RearAttackShip::update(double deltaTime, PlayerShip* player) {

	timeAlive += deltaTime;

	//if (timeAlive <= timeToClimax) {
	double percent = timeAlive / timeToClimax;
	//percent = sin(percent*XM_PIDIV2);
	//position = Vector2::Lerp(startPos, climaxPos, percent);
	percent = 1 - cos(percent*XM_PIDIV2);
	double rt = 1 - percent;
	position = rt*rt*startPos + 2 * rt*percent*controlPoint + percent*percent*climaxPos;



	//} else if (fired) {
	//	double percent = (timeAlive - timeToClimax) / timeToEnd;
	//	percent = 1 - cos(percent*XM_PIDIV2);
	//	//percent *= percent;
	//	position = Vector2::Lerp(climaxPos, endPos, percent);

	if (position.y > Globals::WINDOW_HEIGHT + 120) {
		isAlive = false;
	}

	if (!fired && timeAlive >= timeToFire) {
		fireReady = true;
		fired = true;
	}

	weaponLocation = position;

	EnemyShip::update(deltaTime);
}


