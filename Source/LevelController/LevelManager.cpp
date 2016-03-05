#include "LevelManager.h"

LevelManager::LevelManager(ID3D11Device* device) {

	arial.reset(new FontSet());
	arial->load(device, L"assets/Arial.spritefont");
	arial->layerDepth = .1f;
	arial->setTint(DirectX::Colors::Black.v);

	pauseFont.reset(new FontSet());
	pauseFont->load(device, L"assets/Arial.spritefont");
	pauseFont->layerDepth = .1f;
	pauseFont->setTint(DirectX::Colors::Black.v);

	waveManager.reset(new WaveManager());

}

LevelManager::~LevelManager() {

}

bool LevelManager::initStage(ID3D11Device* device) {


	playerShip.reset(new PlayerShip(startPosition));
	playerShip->layerDepth = .25f;
	if (!playerShip->load(device, L"assets/Heroship Complete.dds")) {
		MessageBox(NULL, L"Failed to load playership", L"ERROR", MB_OK);
		return false;
	}
	if (!playerShip->loadBullet(device)) {
		MessageBox(NULL, L"Failed to load weapons", L"ERROR", MB_OK);
		return false;
	}

	
	waveManager->initialize(device);


	mouse.reset(new Sprite(SimpleMath::Vector2(1, 1)));
	mouse->layerDepth = .5f;
	if (!mouse->load(device, L"assets/mouse icon.dds"))
		return false;


	timerLabel.reset(new TextLabel(Vector2(500, 10), arial.get()));
	textLabels.push_back(timerLabel.get());

	scoreLabel.reset(new TextLabel(Vector2(10, 10), arial.get()));
	textLabels.push_back(scoreLabel.get());

	shipPosLabel.reset(new TextLabel(Vector2(10, 30), arial.get()));
	textLabels.push_back(shipPosLabel.get());

	pauseLabel.reset(new TextLabel(
		Vector2(Globals::WINDOW_WIDTH / 2, Globals::WINDOW_HEIGHT / 2),
		pauseFont.get()));
	wostringstream ws;
	ws << "Paused";
	pauseLabel->label = ws.str();

	return true;
}





void LevelManager::update(double deltaTime, BYTE keyboardState[256], DIMOUSESTATE mouseCurrentState, Vector2 mousePos) {

	// ship updates
	playerShip->update(deltaTime, keyboardState, mouseCurrentState, mousePos);
	waveManager->update(deltaTime, playerShip.get());

	// check collisions 


	// GUI updates
	mouse->setPostion(mousePos);
	//{
	//	wostringstream ws;
	//	ws << "Score: " << scoreLabel ;
	//	scoreLabel->label = ws.str();
	//}

	{
		wostringstream ws;
		ws << "liveBullets: " << playerShip->liveBullets.size();
		scoreLabel->label = ws.str();
	}


	{
		wostringstream ws;
		ws << "Time: " << deltaTime << "s";
		timerLabel->label = ws.str();
	}
	//const HitArea* shipHitArea = playerShip->getHitArea();

	/*{
		wostringstream ws;
		ws << "shipHitArea X: " << shipHitArea->position.x << " shipHitArea Y: " << shipHitArea->position.y;
		shipPosLabel->label = ws.str();
	}*/

	{
		wostringstream ws;
		ws << "Engery: " << playerShip->energy;
		shipPosLabel->label = ws.str();
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

void LevelManager::pauseScreen(double deltaTime, SpriteBatch* batch) {


	draw(batch);

	Color color = pauseFont->getTint();
	float r = color.R();
	float g = color.G();
	float b = color.B();

	if (rInc)
		r += .001;
	else
		r -= .001;

	if (r >= 1)
		rInc = false;
	if (r <= 0)
		rInc = true;

	/*if (gInc)
		g += .0001;
	else
		g -= .0001;

	if (g >= 1)
		gInc = false;
	if (g <= 0)
		gInc = true;

	if (bInc)
		b += .00001;
	else
		b -= .00001;

	if (b >= 1)
		bInc = false;
	if (b <= 0)
		bInc = true;*/

	pauseFont->setTint(Color(Vector3(r, 0, 0)));
	pauseLabel->draw(batch);
}
