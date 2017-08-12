#include "../pch.h"
#include "LevelManager.h"
#include "../assets.h"
#include "GameManager.h"
#include "../Engine/GameEngine.h"

LevelManager::~LevelManager() {

}

void LevelManager::setGameManager(GameManager* gm) {
	game = gm;
}


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


	if (!waveManager.initialize(gfxAssets.get()))
		return false;

	playerShip.reset(new PlayerShip(PLAYER_START_POSITION, mouse));
	playerShip->load(gfxAssets->getAsset("PlayerShip Hull"));
	if (!playerShip->loadBullet(gfxAssets.get())) {
		MessageBox(NULL, L"Failed to load weapons", L"ERROR", MB_OK);
		return false;
	}
	playerShip->setDimensions(playerShip.get());


	guiOverlay = make_unique<GUIOverlay>();
	guiOverlay->exitButton->setActionListener(new ExitButtonListener(this));
	guiOverlay->continueButton->setActionListener(new ContinueButtonListener(this));

	Vector2 viewarea = guiOverlay->getPlayArea(); // for some reason this step is necessary
	Vector2 viewposition = guiOverlay->getPlayPosition();
	camera->updateViewport(viewarea, viewposition);

	return true;
}

void LevelManager::reloadGraphicsAssets() {
	bgManager.reloadGraphicsAssets();
	waveManager.reloadGraphicsAssets();
	guiOverlay->reloadGraphicsAssets();
}


bool LevelManager::loadLevel(ComPtr<ID3D11Device> device, const char_t* levelFileName) {

	playState = LOADING;

	bgManager.clear();
	if (!bgManager.loadLevel(device, levelFileName))
		return false;

	waveManager.clear();
	if (!waveManager.initialize(gfxAssets.get()))
		return false;

	totalPlayTime = 0;
	gameOverTimer = 0;
	playerShip->reset();

	playState = STARTING;
	return true;
}


void LevelManager::update(double deltaTime) {

	//auto keyState = Keyboard::Get().GetState();
	switch (playState) {
		case PLAYING:
			totalPlayTime += deltaTime;

			// player's bullet hit detection
			for (Bullet* bullet : playerShip->liveBullets) {
				bullet->update(deltaTime);
				for (Wave* wave : waveManager.waves) {
					for (EnemyShip* enemy : wave->shipStore) {
						if (enemy->isAlive) {
							if (bullet->getHitArea()->collision(enemy->getHitArea())) {
								enemy->takeDamage(bullet->damage);
								bullet->isAlive = false;
							}
						}
					}
				}

				for (ClothLayer* layer : bgManager.getClothes()) {
					if (layer->isAlive()) {
						if (bullet->getHitArea()->collision(layer->getHitArea())) {
							layer->takeDamage(bullet->damage);
							bullet->isAlive = false;
						}
					}
				}
			}


			if (bgManager.update(deltaTime)) {
				playState = PlayState::FINISHED;
				waveManager.clearEnemies();
			}
			playerShip->update(deltaTime);
			waveManager.update(deltaTime, playerShip.get());


			//if (!pauseDownLast && keyState.Escape) {
			if (keys->isKeyPressed(Keyboard::Escape)) {
				playState = PAUSED;
				//pauseDownLast = true;
			}

			if (!playerShip->isAlive) {
				playState = PlayState::GAMEOVER;
			}
			break;
		case STARTING:
			if (bgManager.startUpdate(deltaTime)) {
				if (playerShip->startUpdate(deltaTime)) {
					if (delayedPause) {
						playState = PAUSED;
						delayedPause = false;
					} else
						playState = PLAYING;
				}
			}
			guiOverlay->updateWarning(deltaTime);
			break;
		case FINISHED:
			for (Bullet* bullet : playerShip->liveBullets)
				bullet->update(deltaTime);
			waveManager.finishedUpdate(deltaTime);
			playerShip->finishedUpdate(deltaTime);

			gameOverTimer += deltaTime;
			if (gameOverTimer > 15 /*|| keyState.Escape || keyState.Enter*/
				|| keys->isKeyPressed(Keyboard::Escape)
				|| keys->isKeyPressed(Keyboard::Enter))
				game->loadMainMenu();
			break;
		case GAMEOVER:
			for (Bullet* bullet : playerShip->liveBullets)
				bullet->update(deltaTime);
			//bgManager->update(deltaTime, mouse);
			playerShip->deathUpdate(deltaTime);
			waveManager.update(deltaTime, playerShip.get());
			gameOverTimer += deltaTime;
			if (gameOverTimer > 15 /*|| keyState.Escape || keyState.Enter)*/
				|| keys->isKeyPressed(Keyboard::Escape)
				|| keys->isKeyPressed(Keyboard::Enter))
				game->loadMainMenu();
			break;
		case PAUSED:
			guiOverlay->updatePaused(deltaTime);
			//if (!pauseDownLast && keyState.Escape) {
			if (keys->isKeyPressed(Keyboard::Escape)) {
				playState = PLAYING;
				//pauseDownLast = true;
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

	//pauseDownLast = keyState.Escape;
}


void LevelManager::draw(SpriteBatch* batch) {

	batch->Begin(SpriteSortMode_Deferred, NULL, NULL, NULL, NULL, NULL, camera->translationMatrix());
	{
		bgManager.draw(batch);

	}
	batch->End();



	batch->Begin(SpriteSortMode_Deferred);
	{


		waveManager.draw(batch);



		switch (playState) {
			case PAUSED:
				playerShip->draw(batch);
				guiOverlay->drawPaused(batch);
				break;
			case STARTING:
				playerShip->draw(batch);
				if (bgManager.introScrollDone)
					guiOverlay->drawWarning(batch);
				break;
			case GAMEOVER:
				playerShip->deathDraw(batch);
				guiOverlay->drawGameOver(batch);
				break;
			default:
				playerShip->draw(batch);
		}

		guiOverlay->draw(batch);
	}
	batch->End();
}

//void LevelManager::safedraw(SpriteBatch* batch) {
//	bgManager->draw(batch);
//
//
//	playerShip->draw(batch);
//	waveManager->draw(batch);
//}

void LevelManager::pause() {

	if (playState != STARTING)
		playState = PAUSED;
	else {
		delayedPause = true;
	}
}


void LevelManager::controllerRemoved(ControllerSocketNumber controllerSlot,
	PlayerSlotNumber slotNumber) {
}

void LevelManager::newController(shared_ptr<Joystick> newStick) {
}


void LevelManager::resume() {
	playState = PLAYING;
	//pauseDownLast = true;
}

void LevelManager::exitLevel() {

	waveManager.clear();
	bgManager.clear();
	//playerShip->clear();


	//pauseDownLast = true;

	game->loadMainMenu();
}

void ExitButtonListener::onClick(Button* button) {

	lvlManager->exitLevel();
}

void ExitButtonListener::onPress(Button * button) {
}

void ExitButtonListener::onHover(Button * button) {
}

void ExitButtonListener::resetState(Button * button) {
}

void ContinueButtonListener::onClick(Button * button) {

	lvlManager->resume();
}

void ContinueButtonListener::onPress(Button * button) {
}

void ContinueButtonListener::onHover(Button * button) {
}

void ContinueButtonListener::resetState(Button * button) {
}
