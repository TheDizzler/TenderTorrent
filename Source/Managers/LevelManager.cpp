#include "LevelManager.h"

LevelManager::LevelManager() {
}

LevelManager::~LevelManager() {

	/*for (TextLabel* label : textLabels)
		delete label;*/
	//textLabels.clear();
}

void LevelManager::setGameManager(GameManager* gm) {
	game = gm;
}

#include "../assets.h"
#include "GameManager.h"
bool LevelManager::initialize(ComPtr<ID3D11Device> device, MouseController* mouse) {

	levelManifest.reset(new xml_document());
	xml_parse_result result = levelManifest->load_file(Assets::levelManifestFile);

	if (!result) {
		wostringstream wss;
		wss << "Could not read Level Manifest file!";
		wss << "\n" << result.description();
		MessageBox(0, wss.str().c_str(), L"Fatal Read Error!", MB_OK);
		return false;
	}

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
	if (!waveManager->initialize(GameManager::gfxAssets.get()))
		return false;

	playerShip.reset(new PlayerShip(startPosition));
	playerShip->load(GameManager::gfxAssets->getAsset("PlayerShip Hull"));
	if (!playerShip->loadBullet(GameManager::gfxAssets.get())) {
		MessageBox(NULL, L"Failed to load weapons", L"ERROR", MB_OK);
		return false;
	}
	playerShip->setDimensions(playerShip.get());



	timerLabel.reset(new TextLabel(Vector2(500, 10),
		GameManager::guiFactory->getFont("Arial")));
	//textLabels.push_back(timerLabel.get());

	scoreLabel.reset(new TextLabel(Vector2(10, 10),
		GameManager::guiFactory->getFont("Arial")));
	//textLabels.push_back(scoreLabel.get());

	energyLabel.reset(new TextLabel(Vector2(10, 30),
		GameManager::guiFactory->getFont("Arial")));
	//textLabels.push_back(energyLabel.get());



	/*pauseOverlay.reset(new Sprite(Vector2(0, 0)));
	if (!pauseOverlay->load(device, Assets::pauseOverlayFile)) {
		MessageBox(NULL, L"Failed to pause overlay", L"ERROR", MB_OK);
		return false;
	}*/



	return true;
}


bool LevelManager::loadLevel(ComPtr<ID3D11Device> device, const char_t* levelFileName) {

	playState = LOADING;

	pauseOverlay.reset(
		GameManager::guiFactory->createRectangle(Vector2(0, 0),
			Vector2(Globals::WINDOW_WIDTH, Globals::WINDOW_HEIGHT)));
	pauseOverlay->setTint(Color(1, .588, 1, .8)); //should be pinkish


	pauseLabel.reset(new TextLabel(
		GameManager::guiFactory->getFont("BlackCloak")));
	pauseLabel->setText(L"Paused");
	pauseLabel->setScale(Vector2(1, 1.5));
	Vector2 size = pauseLabel->measureString();
	pauseLabel->setPosition(Vector2(
		(Globals::WINDOW_WIDTH - size.x) / 2, (Globals::WINDOW_HEIGHT - size.y) / 2));


	exitButton.reset(
		GameManager::guiFactory->createImageButton(
			Vector2(Globals::WINDOW_WIDTH / 4, Globals::WINDOW_HEIGHT * 3 / 4),
			"Button Up", "Button Down"));
	exitButton->setText(L"Exit");
	Vector2 moveBy = Vector2(exitButton->getScaledWidth() / 2, 0);
	exitButton->moveBy(-moveBy);



	continueButton.reset(
		GameManager::guiFactory->createImageButton(
			Vector2(Globals::WINDOW_WIDTH * 3 / 4, Globals::WINDOW_HEIGHT * 3 / 4),
			"Button Up", "Button Down"));
	continueButton->setText(L"Continue");
	continueButton->moveBy(-moveBy);




	warningLabel.reset(new TextLabel(
		Vector2((Globals::WINDOW_WIDTH) / 2, (Globals::WINDOW_HEIGHT) / 2),
		GameManager::guiFactory->getFont("BlackCloak")));
	warningLabel->setText("GET READY!");
	warningLabel->setScale(Vector2(1, 1.5));
	size = warningLabel->measureString(L"GET READY!");
	warningLabel->moveBy(-size / 2);


	if (!bgManager->loadLevel(device, levelFileName))
		return false;

	waveManager.reset(new WaveManager());
	if (!waveManager->initialize(GameManager::gfxAssets.get()))
		return false;

	totalPlayTime = 0;
	playerShip->reset();

	playState = STARTING;
	return true;
}


void LevelManager::update(double deltaTime,
	KeyboardController* keys, MouseController* mouse) {

	auto keyState = Keyboard::Get().GetState();
	switch (playState) {
		case PLAYING:
			totalPlayTime += deltaTime;

			// player's bullet hit detection
			for (Bullet* bullet : playerShip->liveBullets) {
				bullet->update(deltaTime);
				for (Wave* wave : waveManager->waves) {
					for (EnemyShip* enemy : wave->shipStore) {
						if (enemy->isAlive) {
							if (bullet->getHitArea()->collision(enemy->getHitArea())) {
								enemy->takeDamage(bullet->damage);
								bullet->isAlive = false;
							}
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

			if (!pauseDownLast && keyState.Escape) {
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
				//textLabels.pop_back();
			} else
				displayWarning(deltaTime);
			break;
		case PAUSED:
			displayPause(deltaTime);

			exitButton->update(deltaTime);
			if (exitButton->clicked()) {
				waveManager->clear();
				bgManager->clear();
				//playerShip->clear();


				pauseDownLast = true;

				game->loadMainMenu();
			}

			continueButton->update(deltaTime);
			if (continueButton->clicked()
				|| (!pauseDownLast && keyState.Escape)) {
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

	pauseDownLast = keyState.Escape;
}


void LevelManager::draw(SpriteBatch* batch) {

	bgManager->draw(batch);


	timerLabel->draw(batch);
	scoreLabel->draw(batch);
	energyLabel->draw(batch);


	playerShip->draw(batch);
	waveManager->draw(batch);


	if (playState == PAUSED) {
		pauseOverlay->draw(batch);
		exitButton->draw(batch);
		continueButton->draw(batch);
		pauseLabel->draw(batch);

	} else if (playState == STARTING)
		warningLabel->draw(batch);
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

	//Color color = warningFont->getTint();
	Color color = warningLabel->getTint();
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

	//warningFont->setTint(Color(Vector3(1, r, r)));
	warningLabel->setTint(Vector4(1, r, r, 1));
}

/** Changes the Red variable between 0 and 1. */
void LevelManager::displayPause(double deltaTime) {

	Color color = pauseLabel->getTint();

	if (rInc) {
		color.R(color.R() + deltaTime);
		if (color.R() >= 1)
			rInc = false;
	} else {
		color.R(color.R() - deltaTime);
		if (color.R() <= 0)
			rInc = true;
	}

	pauseLabel->setTint(color);
}

