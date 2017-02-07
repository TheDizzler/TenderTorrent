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
bool LevelManager::initialize(ComPtr<ID3D11Device> device, shared_ptr<MouseController> mouse) {

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

	playerShip.reset(new PlayerShip(PLAYER_START_POSITION));
	playerShip->load(GameManager::gfxAssets->getAsset("PlayerShip Hull"));
	if (!playerShip->loadBullet(GameManager::gfxAssets.get())) {
		MessageBox(NULL, L"Failed to load weapons", L"ERROR", MB_OK);
		return false;
	}
	playerShip->setDimensions(playerShip.get());


	guiOverlay = make_unique<GUIOverlay>();
	guiOverlay->exitButton->setOnClickListener(new ExitButtonListener(this));
	guiOverlay->continueButton->setOnClickListener(new ContinueButtonListener(this));


	return true;
}


bool LevelManager::loadLevel(ComPtr<ID3D11Device> device, const char_t* levelFileName) {

	playState = LOADING;

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


void LevelManager::update(double deltaTime, shared_ptr<MouseController> mouse) {

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
			playerShip->update(deltaTime, mouse);
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
			} else
				guiOverlay->updateWarning(deltaTime);
			break;
		case PAUSED:
			guiOverlay->updatePaused(deltaTime);
			if (!pauseDownLast && keyState.Escape) {
				playState = PLAYING;
				pauseDownLast = true;
			}

			break;

	}
	// GUI updates
	{
		wostringstream ws;
		ws << "Score: " << score;
		guiOverlay->scoreLabel->setText(ws);
	}

	{
		wostringstream ws;
		ws << "Time: " << (int) totalPlayTime << "s";
		guiOverlay->timerLabel->setText(ws);
	}


	{
		wostringstream ws;
		ws << "Energy: " << playerShip->energy;
		guiOverlay->energyLabel->setText(ws);
	}

	pauseDownLast = keyState.Escape;
}


void LevelManager::draw(SpriteBatch* batch) {

	bgManager->draw(batch);


	playerShip->draw(batch);
	waveManager->draw(batch);

	guiOverlay->draw(batch);


	if (playState == PAUSED) {
		guiOverlay->drawPaused(batch);
	} else if (playState == STARTING)
		guiOverlay->drawWarning(batch);
}

void LevelManager::pause() {

	if (playState != STARTING)
		playState = PAUSED;
	else {
		delayedPause = true;
	}
}


void LevelManager::resume() {
	playState = PLAYING;
	pauseDownLast = true;
}

void LevelManager::exitLevel() {

	waveManager->clear();
	bgManager->clear();
	//playerShip->clear();


	pauseDownLast = true;

	game->loadMainMenu();
}

void ExitButtonListener::onClick(Button* button) {

	lvlManager->exitLevel();
}

void ContinueButtonListener::onClick(Button * button) {

	lvlManager->resume();
}
