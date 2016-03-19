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
	warningFont->setScale(Vector2(2, 2));


	if (!mouse->load(device, Assets::mouseReticle))
		return false;

	bgManager.reset(new BackgroundManager());
	if (!bgManager->initialize(device))
		return false;

	waveManager.reset(new WaveManager());
	if (!waveManager->initialize(device))
		return false;

	playerShip.reset(new PlayerShip(startPosition));
	if (!playerShip->load(device, L"assets/Heroship.dds")) {
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

	exitButton.reset(new Button());
	if (!exitButton->load(device, Assets::arialFontFile,
		Assets::buttonUpFile, Assets::buttonDownFile))
		return false;
	exitButton->action = ButtonAction::EXIT;
	exitButton->setText("Exit");
	exitButton->setPosition(
		Vector2(Globals::WINDOW_WIDTH / 4, Globals::WINDOW_HEIGHT * 3 / 4));

	continueButton.reset(new Button());
	if (!continueButton->load(device, Assets::arialFontFile,
		Assets::buttonUpFile, Assets::buttonDownFile))
		return false;
	continueButton->action = ButtonAction::CANCEL_BUTTON;
	continueButton->setText("Continue");
	continueButton->setPosition(
		Vector2(Globals::WINDOW_WIDTH * 3 / 4, Globals::WINDOW_HEIGHT * 3 / 4));


	size = warningFont->measureString(L"GET READY!");
	warningLabel.reset(new TextLabel(
		Vector2((Globals::WINDOW_WIDTH - size.x) / 2, (Globals::WINDOW_HEIGHT - size.y) / 2),
		warningFont.get()));
	warningLabel->setText("GET READY!");
	textLabels.push_back(warningLabel.get());


	pauseOverlay.reset(new Sprite(Vector2(0, 0)));
	if (!pauseOverlay->load(device, Assets::pauseOverlayFile)) {
		MessageBox(NULL, L"Failed to pause overlay", L"ERROR", MB_OK);
		return false;
	}
	pauseOverlay->setTint(Color(Vector4(1, 1, 1, .25)));


	playState = STARTING;

	return true;
}

#include "GameManager.h"

void LevelManager::update(double deltaTime, BYTE keyboardState[256], MouseController* mouse) {

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
						layer->takeDamage(bullet->damage);
						bullet->isAlive = false;
					}
				}
			}


			bgManager->update(deltaTime, playerShip.get());
			playerShip->update(deltaTime, keyboardState, mouse);
			waveManager->update(deltaTime, playerShip.get());

			if (!pauseDownLast && (keyboardState[DIK_P] || keyboardState[DIK_ESCAPE])) {
				textLabels.push_back(pauseLabel.get());
				playState = PAUSED;
				pauseDownLast = true;
			}

			if (!playerShip->isAlive)
				game->loadMainMenu();
			break;
		case STARTING:
			bgManager->startUpdate(deltaTime);
			if (playerShip->startUpdate(deltaTime, mouse)) {
				playState = PLAYING;
				textLabels.pop_back();
			} else
				displayWarning(deltaTime);
			break;
		case PAUSED:
			displayPause(deltaTime);

			exitButton->update(deltaTime, mouse);
			if (exitButton->clicked())
				game->loadMainMenu();

			continueButton->update(deltaTime, mouse);
			if (continueButton->clicked()
				|| (!pauseDownLast && (keyboardState[DIK_P] || keyboardState[DIK_ESCAPE]))) {
				playState = PLAYING;
				textLabels.pop_back();
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
		ws << "Engery: " << playerShip->energy;
		energyLabel->setText(ws);
	}

	pauseDownLast = keyboardState[DIK_P] || keyboardState[DIK_ESCAPE];
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
