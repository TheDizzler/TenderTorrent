#include "LevelManager.h"

LevelManager::LevelManager() {
}

LevelManager::~LevelManager() {

	for (TextLabel* label : textLabels)
		delete label;
	textLabels.clear();
}

void LevelManager::setGameManager(GameManager * gm) {

	game = gm;
}

//#include "Ga
bool LevelManager::initialize(ComPtr<ID3D11Device> device, MouseController* mouse) {

	playState = LOADING;

	/*guiFont.reset(new FontSet());
	if (!guiFont->load(device, Assets::arialFontFile))
		return false;
	guiFont->setTint(DirectX::Colors::Black.v);

	pauseFont.reset(new FontSet());
	if (!pauseFont->load(device, Assets::arialFontFile))
		return false;
	pauseFont->setTint(Color(Vector3(0, 1, 1)));
	pauseFont->setScale(Vector2(1.5, 1.5));

	warningFont.reset(new FontSet());
	if (!warningFont->load(device, Assets::arialFontFile))
		return false;
	warningFont->setTint(DirectX::Colors::White.v);
	warningFont->setScale(Vector2(2, 2));*/


	if (!mouse->loadMouseIcon(GameManager::guiFactory.get(), "Mouse Reticle"))
		return false;

	bgManager.reset(new BackgroundManager());

	waveManager.reset(new WaveManager());
	if (!waveManager->initialize(device))
		return false;

	playerShip.reset(new PlayerShip(startPosition));
	playerShip->load(GameManager::guiFactory->getAsset("PlayerShip Hull"));
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

	exitButton.reset(new TextButton());
	if (!exitButton->load(device, Assets::arialFontFile,
		Assets::buttonUpFile, Assets::buttonDownFile))
		return false;
	exitButton->action = Button::EXIT;
	exitButton->setText("Exit");
	exitButton->setPosition(
		Vector2(Globals::WINDOW_WIDTH / 4, Globals::WINDOW_HEIGHT * 3 / 4));

	continueButton.reset(new TextButton());
	if (!continueButton->load(device, Assets::arialFontFile,
		Assets::buttonUpFile, Assets::buttonDownFile))
		return false;
	continueButton->action = Button::CANCEL;
	continueButton->setText("Continue");
	continueButton->setPosition(
		Vector2(Globals::WINDOW_WIDTH * 3 / 4, Globals::WINDOW_HEIGHT * 3 / 4));


	size = warningFont->measureString(L"GET READY!");
	warningLabel.reset(new TextLabel(
		Vector2((Globals::WINDOW_WIDTH - size.x) / 2, (Globals::WINDOW_HEIGHT - size.y) / 2),
		warningFont.get()));
	warningLabel->setText("GET READY!");


	pauseOverlay.reset(new Sprite(Vector2(0, 0)));
	if (!pauseOverlay->load(device, Assets::pauseOverlayFile)) {
		MessageBox(NULL, L"Failed to pause overlay", L"ERROR", MB_OK);
		return false;
	}
	pauseOverlay->setTint(Color(Vector4(1, 1, 1, .25)));

	return true;
}

bool LevelManager::loadLevel(ComPtr<ID3D11Device> device, const wchar_t* file) {

	if (!bgManager->loadLevel(device, file))
		return false;

	waveManager.reset(new WaveManager());
	if (!waveManager->initialize(device))
		return false;

	totalPlayTime = 0;
	playerShip->energy = playerShip->maxEnergy;

	textLabels.push_back(warningLabel.get());
	playState = STARTING;
	return true;
}

#include "GameManager.h"

void LevelManager::update(double deltaTime, KeyboardController* keys, MouseController* mouse) {

	switch (playState) {
		case PLAYING:
			totalPlayTime += deltaTime;

			// player's bullet hit detection
			for (Bullet* bullet : playerShip->liveBullets) {
				bullet->update(deltaTime);
				for (Wave* wave : waveManager->waves) {
					for (EnemyShip* enemy : wave->enemyShips) {
						if (bullet->getHitArea()->collision(enemy->getHitArea())) {
							enemy->takeDamage(bullet->damage);
							bullet->isAlive = false;
						}
					}
				}

				for (BackgroundLayer* layer : bgManager->getLayers()) {

					if (bullet->getHitArea()->collision(layer->getHitArea())) {
						if (layer->isAlive) {
							layer->takeDamage(bullet->damage);
							bullet->isAlive = false;
						}
					}
				}
			}


			bgManager->update(deltaTime, playerShip.get());
			playerShip->update(deltaTime, keys, mouse);
			waveManager->update(deltaTime, playerShip.get());

			if (!pauseDownLast && keys->keyDown[KeyboardController::PAUSE]) {
			//if (!keys->lastDown[KeyboardController::CANCEL]
				//&& keys->keyDown[KeyboardController::CANCEL]) {

				playState = PAUSED;
				pauseDownLast = true;
			}

			if (!playerShip->isAlive)
				game->loadMainMenu();
			break;
		case STARTING:

			if (playerShip->startUpdate(deltaTime, mouse)) {
				if (delayedPause) {
					playState = PAUSED;
					delayedPause = false;
				} else
					playState = PLAYING;
				textLabels.pop_back();
			} else
				displayWarning(deltaTime);
			break;
		case PAUSED:
			displayPause(deltaTime);

			exitButton->update(deltaTime, mouse);
			if (exitButton->clicked()) {
				waveManager->clear();
				bgManager->clear();
				playerShip->clear();


				pauseDownLast = true;

				game->loadMainMenu();
			}

			continueButton->update(deltaTime, mouse);
			if (continueButton->clicked()
				|| (!pauseDownLast && keys->keyDown[KeyboardController::PAUSE])) {
				//|| (!keys->lastDown[KeyboardController::CANCEL]
					//&& keys->keyDown[KeyboardController::CANCEL])) {
				playState = PLAYING;
				pauseDownLast = true;
			}
			break;

	}
	// GUI updates
	{
		wostringstream ws;
		ws << "Score: " << score;
		scoreLabel->setText(ws);
	}

	{
		wostringstream ws;
		ws << "Time: " << (int) totalPlayTime << "s";
		timerLabel->setText(ws);
	}


	{
		wostringstream ws;
		ws << "Energy: " << playerShip->energy;
		energyLabel->setText(ws);
	}

	pauseDownLast = keys->keyDown[KeyboardController::PAUSE];
}


void LevelManager::draw(SpriteBatch* batch) {

	bgManager->draw(batch);

	for (auto const& label : textLabels)
		label->draw(batch);

	playerShip->draw(batch);
	waveManager->draw(batch);


	if (playState == PAUSED) {
		for (int w = 0; w <= Globals::WINDOW_WIDTH; w += 16)
			for (int h = 0; h <= Globals::WINDOW_HEIGHT; h += 16) {
				pauseOverlay->setPosition(Vector2(w, h));
				pauseOverlay->draw(batch);
			}
		exitButton->draw(batch);
		continueButton->draw(batch);
		pauseLabel->draw(batch);

	}


}

void LevelManager::pause() {

	if (playState != STARTING)
		playState = PAUSED;
	else {
		delayedPause = true;
	}
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

