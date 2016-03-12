#include "LevelManager.h"

LevelManager::LevelManager() {
}

LevelManager::~LevelManager() {
}

void LevelManager::setGameManager(GameManager * gm) {

	game = gm;
}

bool LevelManager::initialize(ID3D11Device* device, MouseController* mouse) {

	playState = LOADING;

	guiFont.reset(new FontSet());
	if (!guiFont->load(device, L"assets/Arial.spritefont"))
		return false;
	guiFont->setTint(DirectX::Colors::Black.v);

	pauseFont.reset(new FontSet());
	if (!pauseFont->load(device, L"assets/Arial.spritefont"))
		return false;
	pauseFont->setTint(Color(Vector3(0, 1, 1)));
	pauseFont->setScale(Vector2(1.5, 1.5));

	warningFont.reset(new FontSet());
	if (!warningFont->load(device, L"assets/Arial.spritefont"))
		return false;
	warningFont->setTint(DirectX::Colors::White.v);
	warningFont->setScale(Vector2(2, 2));


	if (!mouse->load(device, L"assets/reticle (16x16).dds"))
		return false;


	waveManager.reset(new WaveManager());
	waveManager->initialize(device);

	playerShip.reset(new PlayerShip(startPosition));
	if (!playerShip->load(device, L"assets/Heroship Complete.dds")) {
		MessageBox(NULL, L"Failed to load playership", L"ERROR", MB_OK);
		return false;
	}
	if (!playerShip->loadBullet(device)) {
		MessageBox(NULL, L"Failed to load weapons", L"ERROR", MB_OK);
		return false;
	}
	playerShip->setDimensions(playerShip.get());





	timerLabel.reset(new TextLabel(Vector2(500, 10), guiFont.get()));
	textLabels.push_back(timerLabel.get());

	scoreLabel.reset(new TextLabel(Vector2(10, 10), guiFont.get()));
	textLabels.push_back(scoreLabel.get());

	energyLabel.reset(new TextLabel(Vector2(10, 30), guiFont.get()));
	textLabels.push_back(energyLabel.get());

	Vector2 size = pauseFont->measureString(L"Paused");
	pauseLabel.reset(new TextLabel(
		Vector2((Globals::WINDOW_WIDTH - size.x) / 2, (Globals::WINDOW_HEIGHT - size.y) / 2),
		pauseFont.get()));
	pauseLabel->setText("Paused");


	size = warningFont->measureString(L"GET READY!");
	warningLabel.reset(new TextLabel(
		Vector2((Globals::WINDOW_WIDTH - size.x) / 2, (Globals::WINDOW_HEIGHT - size.y) / 2),
		warningFont.get()));
	warningLabel->setText("GET READY!");
	textLabels.push_back(warningLabel.get());


	playState = STARTING;

	return true;
}

#include "GameManager.h"

void LevelManager::update(double deltaTime, BYTE keyboardState[256], MouseController* mouse) {

	switch (playState) {
		case PLAYING:
			totalPlayTime += deltaTime;

			playerShip->update(deltaTime, keyboardState, mouse);
			waveManager->update(deltaTime, playerShip.get());

			pauseDelay += deltaTime;
			if (pauseDelay >= .25 && keyboardState[DIK_P]) {
				textLabels.push_back(pauseLabel.get());
				playState = PAUSED;
				pauseDelay = 0;
			}

			if (!playerShip->isAlive)
				game->loadMainMenu();
			break;
		case STARTING:
			if (playerShip->startUpdate(deltaTime)) {
				playState = PLAYING;
				textLabels.pop_back();
			} else
				displayWarning(deltaTime);
			break;
		case PAUSED:
			displayPause(deltaTime);
			pauseDelay += deltaTime;
			if (pauseDelay >= 1 && keyboardState[DIK_P]) {
				playState = PLAYING;
				textLabels.pop_back();
				pauseDelay = 0;
			}
			break;

	}
	// GUI updates
	{
		wostringstream ws;
		ws << "Score: " << score;
		scoreLabel->label = ws.str();
	}

	{
		wostringstream ws;
		ws << "Time: " << (int) totalPlayTime << "s";
		timerLabel->label = ws.str();
	}


	{
		wostringstream ws;
		ws << "Engery: " << playerShip->energy;
		energyLabel->label = ws.str();
	}

}


void LevelManager::draw(SpriteBatch* batch) {


	for (auto const& label : textLabels)
		label->draw(batch);


	playerShip->draw(batch);

	waveManager->draw(batch);



}


bool rInc = true;
bool gInc = false;
bool bInc = true;

/** Changes the Green and Blue variables between 0 and 1. */
void LevelManager::displayWarning(double deltaTime) {

	Color color = warningFont->getTint();
	float r = color.G();
	if (rInc) {
		r += 5 * deltaTime;
		if (r >= 1)
			rInc = false;
	} else {
		r -= 5 * deltaTime;
		if (r <= 0)
			rInc = true;
	}

	warningFont->setTint(Color(Vector3(1, r, r)));
}

/** Changes the Red variable between 0 and 1. */
void LevelManager::displayPause(double deltaTime) {

	Color color = pauseFont->getTint();

	if (rInc) {
		color.R(color.R() + deltaTime);
		if (color.R() >= 1)
			rInc = false;
	} else {
		color.R(color.R() - deltaTime);
		if (color.R() <= 0)
			rInc = true;
	}

	pauseFont->setTint(color);
}
