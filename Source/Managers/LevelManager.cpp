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


bool LevelManager::initialize(ComPtr<ID3D11Device> device) {

	levelManifest.reset(new xml_document());
	xml_parse_result result = levelManifest->load_file(Assets::levelManifestFile);

	if (!result) {
		wostringstream wss;
		wss << "Could not read Level Manifest file!";
		wss << "\n" << result.description();
		GameManager::showErrorDialog(wss.str().c_str(), L"Fatal Read Error!");
		return false;
	}


	if (!mouse.loadMouseIcon(&guiFactory, "Mouse Reticle"))
		return false;


	guiOverlay.exitButton->setActionListener(new ExitButtonListener(this));
	guiOverlay.continueButton->setActionListener(new ContinueButtonListener(this));


	Vector2 viewarea = guiOverlay.getPlayArea(); // for some reason this step is necessary
	Vector2 viewposition = guiOverlay.getPlayPosition();
	testFrame.reset(guiFactory.createRectangleFrame(viewposition, viewarea, 14, Color(.94f, .97f, 1, 1)));

	camera.updateViewport(viewarea, viewposition);


	CD3D11_RASTERIZER_DESC rsDesc(D3D11_FILL_SOLID, D3D11_CULL_BACK, FALSE,
		0, 0.f, 0.f, TRUE, TRUE, TRUE, FALSE);
	if (GameEngine::reportError(
		device->CreateRasterizerState(&rsDesc, rasterState.GetAddressOf()),
		L"Error creating RasterizerState.", L"ERROR")) {

		return false;
	}

	game->setScissorRect(viewarea, viewposition);

	return true;
}

void LevelManager::reloadGraphicsAssets() {
	bgManager.reloadGraphicsAssets();
	waveManager.reloadGraphicsAssets();
	guiOverlay.reloadGraphicsAssets();
}


bool LevelManager::loadLevel(ComPtr<ID3D11Device> device, const char_t* levelFileName) {

	playState = LOADING;
	totalPlayTime = 0;
	gameOverTimer = 0;

	playerShip.load(gfxAssets.getAsset("PlayerShip Hull"));
	if (!playerShip.loadBullet(&gfxAssets)) {
		MessageBox(NULL, L"Failed to load weapons", L"ERROR", MB_OK);
		return false;
	}
	playerShip.setDimensions(&playerShip);
	playerShip.reset();

	bgManager.clear();
	if (!bgManager.loadLevel(device, levelFileName))
		return false;

	waveManager.clear();
	if (!waveManager.initialize(&gfxAssets))
		return false;


	Vector2 startPos = bgManager.getStartPosition();
	startPos.y += Globals::WINDOW_HEIGHT / 2;
	playerShip.setPosition(startPos);




	playState = STARTING;
	return true;
}


void LevelManager::update(double deltaTime) {

	bool once = true;
	switch (playState) {
		case PLAYING:
			totalPlayTime += deltaTime;

			// player's bullet hit detection
			for (Bullet* bullet : playerShip.liveBullets) {

				bullet->update(deltaTime);
				waveManager.checkHitDetection(bullet);
				/*for (Wave* wave : waveManager.waves) {
					for (EnemyShip* enemy : wave->shipStore) {
						if (enemy->isAlive) {
							if (bullet->getHitArea()->collision(enemy->getHitArea())) {
								enemy->takeDamage(bullet->damage);
								bullet->isAlive = false;
							}
							if (once)
								++liveCount;
						}
					}
				}*/

				for (ClothLayer* layer : bgManager.getClothes()) {
					if (layer->isAlive()) {
						if (bullet->getHitArea()->collision(layer->getHitArea())) {
							layer->takeDamage(bullet->damage);
							bullet->isAlive = false;
						}
					}
				}
				once = false;
			}


			if (bgManager.update(deltaTime)) {
				playState = PlayState::FINISHED;
				waveManager.clearEnemies();
			}
			playerShip.update(deltaTime);
			waveManager.update(deltaTime, &playerShip);


			if (keys.isKeyPressed(Keyboard::Escape) || joystick->startButtonPushed()) {
				playState = PAUSED;
			}

			if (!playerShip.isAlive) {
				playState = PlayState::GAMEOVER;
			}
			break;
		case STARTING:
			if (bgManager.startUpdate(deltaTime)) {
				if (playerShip.startUpdate(deltaTime)) {
					if (delayedPause) {
						playState = PAUSED;
						delayedPause = false;
					} else
						playState = PLAYING;
				}
			}
			guiOverlay.updateWarning(deltaTime);
			break;
		case FINISHED:
			for (Bullet* bullet : playerShip.liveBullets)
				bullet->update(deltaTime);
			waveManager.finishedUpdate(deltaTime);
			playerShip.finishedUpdate(deltaTime);

			gameOverTimer += deltaTime;
			if (gameOverTimer > 15
				|| keys.isKeyPressed(Keyboard::Escape)
				|| keys.isKeyPressed(Keyboard::Enter))
				game->loadMainMenu();
			break;
		case GAMEOVER:
			for (Bullet* bullet : playerShip.liveBullets)
				bullet->update(deltaTime);
			bgManager.update(deltaTime);
			playerShip.deathUpdate(deltaTime);
			waveManager.update(deltaTime, &playerShip);
			gameOverTimer += deltaTime;
			if (gameOverTimer > 15
				|| keys.isKeyPressed(Keyboard::Escape)
				|| keys.isKeyPressed(Keyboard::Enter)
				|| joystick->startButtonPushed())
				game->loadMainMenu();
			break;
		case PAUSED:
			guiOverlay.updatePaused(deltaTime);
			if (keys.isKeyPressed(Keyboard::Escape) || joystick->startButtonPushed()) {
				playState = PLAYING;
			}

			break;

	}
	// GUI updates
	{
		wostringstream ws;
		ws << "Score: " << score;
		guiOverlay.scoreLabel->setText(ws);
	}

	{
		wostringstream ws;
		ws << "Time: " << (int) totalPlayTime << "s";
		guiOverlay.timerLabel->setText(ws);
	}


	{
		wostringstream ws;
		ws << "Energy: " << playerShip.energy;
		guiOverlay.energyLabel->setText(ws);
	}

	/*{
		wostringstream ws;
		ws << "Player: " << playerShip.getPosition().x << ", " << playerShip.getPosition().y;
		guiOverlay->playerLoc->setText(ws);
	}

	{
		wostringstream ws;
		ws << "Camera: " << camera.getPosition().x << ", " << camera.getPosition().y;
		guiOverlay->cameraLoc->setText(ws);
	}*/

	/*{
		wostringstream ws;
		ws << "Bullets: " << waveManager.getBulletCount();
		guiOverlay->bulletCount->setText(ws);
	}*/
	{
		wostringstream ws;
		ws << "Enemies: " << waveManager.getLiveCount();
		guiOverlay.enemyCount->setText(ws);
	}
	/*{
		wostringstream ws;
		ws << "Mouse: " << mouse.getPosition().x << ", " << mouse.getPosition().y;
		guiOverlay->mouseLoc->setText(ws);
	}*/

	testFrame->update();
	guiOverlay.update(deltaTime);
}


void LevelManager::draw(SpriteBatch* batch) {



	switch (playState) {
		case PAUSED:
			batch->Begin(SpriteSortMode_Deferred, NULL, NULL, NULL, rasterState.Get(), NULL,
				camera.translationMatrix());
			{
				bgManager.draw(batch);
				waveManager.draw(batch);
				playerShip.draw(batch);

			}
			batch->End();
			batch->Begin(SpriteSortMode_Deferred);
			{
				guiOverlay.draw(batch);
				guiOverlay.drawPaused(batch);
			}
			batch->End();
			break;
		case STARTING:
			batch->Begin(SpriteSortMode_Deferred, NULL, NULL, NULL, rasterState.Get(), NULL,
				camera.translationMatrix());
			{
				bgManager.draw(batch);
				playerShip.draw(batch);
			}
			batch->End();

			batch->Begin(SpriteSortMode_Deferred);
			{
				if (bgManager.introScrollDone)
					guiOverlay.drawWarning(batch);
				guiOverlay.draw(batch);
			}
			batch->End();
			break;
		case GAMEOVER:
			batch->Begin(SpriteSortMode_Deferred, NULL, NULL, NULL, rasterState.Get(), NULL,
				camera.translationMatrix());
			{
				bgManager.draw(batch);
				playerShip.deathDraw(batch);
			}
			batch->End();

			batch->Begin(SpriteSortMode_Deferred);
			{
				guiOverlay.draw(batch);
				guiOverlay.drawGameOver(batch);
			}
			batch->End();
			break;
		default:
			batch->Begin(SpriteSortMode_Deferred, NULL, NULL, NULL, rasterState.Get(), NULL,
				camera.translationMatrix());
			{
				bgManager.draw(batch);
				waveManager.draw(batch);
				playerShip.draw(batch);
			}
			batch->End();
			batch->Begin(SpriteSortMode_Deferred);
			{
				testFrame->draw(batch);
				guiOverlay.draw(batch);
			}
			batch->End();
	}



}

void LevelManager::textureDraw(SpriteBatch* batch) {

	batch->Begin(SpriteSortMode_Deferred, NULL, NULL, NULL, rasterState.Get(), NULL,
		camera.translationMatrix());
	{
		bgManager.draw(batch);
	}
	batch->End();
}


void LevelManager::pause() {

	if (playState != STARTING)
		playState = PAUSED;
	else {
		delayedPause = true;
	}
}


void LevelManager::controllerRemoved(ControllerSocketNumber controllerSlot,
	PlayerSlotNumber slotNumber) {

	playState = PAUSED;
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

#ifdef _QUICK_LAUNCH
	game->exit();
#endif

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
