#include "LevelManager.h"

LevelManager::LevelManager() {
}

LevelManager::~LevelManager() {

}

void LevelManager::setGameManager(GameManager* gm) {
	game = gm;
}

#include "../assets.h"
#include "GameManager.h"
#include "../Engine/GameEngine.h"
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


	if (!mouse->loadMouseIcon(guiFactory.get(), "Mouse Reticle"))
		return false;

	bgManager.reset(new Background());

	waveManager.reset(new WaveManager());
	if (!waveManager->initialize(gfxAssets.get()))
		return false;

	playerShip.reset(new PlayerShip(PLAYER_START_POSITION));
	playerShip->load(gfxAssets->getAsset("PlayerShip Hull"));
	if (!playerShip->loadBullet(gfxAssets.get())) {
		MessageBox(NULL, L"Failed to load weapons", L"ERROR", MB_OK);
		return false;
	}
	playerShip->setDimensions(playerShip.get());


	guiOverlay = make_unique<GUIOverlay>();
	guiOverlay->exitButton->setOnClickListener(new ExitButtonListener(this));
	guiOverlay->continueButton->setOnClickListener(new ContinueButtonListener(this));

	Vector2 viewarea = guiOverlay->getPlayArea(); // for some reason this step is necessary
	Vector2 viewposition = guiOverlay->getPlayPosition();
	camera->updateViewport(viewarea, viewposition);

	return true;
}


bool LevelManager::loadLevel(ComPtr<ID3D11Device> device, const char_t* levelFileName) {

	playState = LOADING;

	bgManager->clear();
	if (!bgManager->loadLevel(device, levelFileName))
		return false;

	waveManager.reset(new WaveManager());
	if (!waveManager->initialize(gfxAssets.get()))
		return false;

	totalPlayTime = 0;
	gameOverTimer = 0;
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

				//for (BackgroundLayer* layer : bgManager->bgLayers) {
				for (const auto& layer : bgManager->bgLayers) {
					if (bullet->getHitArea()->collision(layer->getHitArea())) {
						//if (layer->isAlive) {
						layer->takeDamage(bullet->damage);
						bullet->isAlive = false;
					//}
					}
				}
			}


			if (bgManager->update(deltaTime, mouse))
				playState = PlayState::FINISHED;
			playerShip->update(deltaTime, mouse);
			waveManager->update(deltaTime, playerShip.get());

			if (!pauseDownLast && keyState.Escape) {
				playState = PAUSED;
				pauseDownLast = true;
			}

			if (!playerShip->isAlive) {
				playState = PlayState::GAMEOVER;
			}
			break;
		case STARTING:
			if (bgManager->startUpdate(deltaTime)) {
				if (playerShip->startUpdate(deltaTime, mouse)) {
					if (delayedPause) {
						playState = PAUSED;
						delayedPause = false;
					} else
						playState = PLAYING;
				} else
					guiOverlay->updateWarning(deltaTime);
			}
			break;
		case FINISHED:
			waveManager->update(deltaTime, playerShip.get());
			gameOverTimer += deltaTime;
			if (gameOverTimer > 5 || keyState.Escape || keyState.Enter)
				game->loadMainMenu();
			break;
		case GAMEOVER:
			bgManager->update(deltaTime, mouse);
			waveManager->update(deltaTime, playerShip.get());
			gameOverTimer += deltaTime;
			if (gameOverTimer > 5 || keyState.Escape || keyState.Enter)
				game->loadMainMenu();
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

#include "../Engine/GameEngine.h"
void LevelManager::draw(SpriteBatch* batch) {

	batch->Begin(SpriteSortMode_Deferred, NULL, NULL, NULL, NULL, NULL, camera->translationMatrix());
	{
		bgManager->draw(batch);

	}
	batch->End();



	batch->Begin(SpriteSortMode_Deferred);
	{

		playerShip->draw(batch);
		waveManager->draw(batch);

		guiOverlay->draw(batch);

		switch (playState) {
			case PAUSED:
				guiOverlay->drawPaused(batch);
				break;
			case STARTING:
				guiOverlay->drawWarning(batch);
				break;
			case GAMEOVER:
				guiOverlay->drawGameOver(batch);
				break;
		}
	}
	batch->End();
}

void LevelManager::safedraw(SpriteBatch* batch) {
	bgManager->draw(batch);


	playerShip->draw(batch);
	waveManager->draw(batch);
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
