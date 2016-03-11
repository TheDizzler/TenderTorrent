#include "LevelManager.h"

LevelManager::LevelManager() {
}

LevelManager::~LevelManager() {
}

void LevelManager::setGameManager(GameManager * gm) {

	game = gm;
}

bool LevelManager::initialize(ID3D11Device* device, MouseController* mouse) {

	guiFont.reset(new FontSet());
	if (!guiFont->load(device, L"assets/Arial.spritefont"))
		return false;
	guiFont->layerDepth = .1f;
	guiFont->setTint(DirectX::Colors::Black.v);

	pauseFont.reset(new FontSet());
	if (!pauseFont->load(device, L"assets/Arial.spritefont"))
		return false;
	pauseFont->layerDepth = .1f;
	pauseFont->setTint(DirectX::Colors::Black.v);


	if (!mouse->load(device, L"assets/mouse icon.dds"))
		return false;


	waveManager.reset(new WaveManager());


	playerShip.reset(new PlayerShip(startPosition));
	//playerShip->layerDepth = .25f;
	if (!playerShip->load(device, L"assets/Heroship Complete.dds")) {
		MessageBox(NULL, L"Failed to load playership", L"ERROR", MB_OK);
		return false;
	}
	if (!playerShip->loadBullet(device)) {
		MessageBox(NULL, L"Failed to load weapons", L"ERROR", MB_OK);
		return false;
	}
	playerShip->setDimensions(playerShip.get());

	waveManager->initialize(device);



	timerLabel.reset(new TextLabel(Vector2(500, 10), guiFont.get()));
	textLabels.push_back(timerLabel.get());

	scoreLabel.reset(new TextLabel(Vector2(10, 10), guiFont.get()));
	textLabels.push_back(scoreLabel.get());

	energyLabel.reset(new TextLabel(Vector2(10, 30), guiFont.get()));
	textLabels.push_back(energyLabel.get());

	pauseLabel.reset(new TextLabel(
		Vector2(Globals::WINDOW_WIDTH / 2, Globals::WINDOW_HEIGHT / 2),
		pauseFont.get()));
	wostringstream ws;
	ws << "Paused";
	pauseLabel->label = ws.str();

	return true;
}





void LevelManager::update(double deltaTime, BYTE keyboardState[256], MouseController* mouse) {

	// ship updates
	playerShip->update(deltaTime, keyboardState, mouse);
	waveManager->update(deltaTime, playerShip.get());


	// GUI updates
	{
		wostringstream ws;
		ws << "Score: " << score;
		scoreLabel->label = ws.str();
	}

	{
		wostringstream ws;
		ws << "Time: " << deltaTime << "s";
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


	mouse->draw(batch);

}

bool rInc = true;
bool gInc = false;
bool bInc = true;

//void LevelManager::updateMenu(double deltaTime, BYTE keyboardState[256], DIMOUSESTATE mouseCurrentState, const Vector2& mousePos) {
//
//
//	if (keyboardState[DIK_ESCAPE]) {
//		waitingForInput = true;
//		if (isPaused) {
//			Color color = pauseFont->getTint();
//			float r = color.R();
//			float g = color.G();
//			float b = color.B();
//
//			if (rInc)
//				r += .001;
//			else
//				r -= .001;
//
//			if (r >= 1)
//				rInc = false;
//			if (r <= 0)
//				rInc = true;
//
//
//			pauseFont->setTint(Color(Vector3(r, 0, 0)));
//			pauseLabel->draw(batch);
//		}
//	}
