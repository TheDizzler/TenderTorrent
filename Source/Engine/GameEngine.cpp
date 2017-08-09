#include "../pch.h"
#include "GameEngine.h"
#include "CommonStates.h"
#include "../DXTKGui/GuiAssets.h"


GameEngine::~GameEngine() {
	
	mouse.reset();
	//game.reset();
	if (audioEngine != NULL)
		audioEngine->Suspend();
}


bool GameEngine::initEngine(HWND hw, HINSTANCE hInstance) {

	hwnd = hw;

	if (!initD3D(hwnd)) {
		MessageBox(0, L"Direct3D Initialization Failed", L"Error", MB_OK);
		return false;
	}

	if (!initRawInput(hwnd)) {
		MessageBox(0, L"Raw Input Init failed", L"Error", MB_OK);
		return false;
	}

	// Initialize Audio Engine
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	AUDIO_ENGINE_FLAGS audioFlags = AudioEngine_Default;
	//#ifdef _DEBUG
	//	audioFlags = audioFlags | AudioEngine_Debug;
	//#endif
	audioEngine = make_unique<AudioEngine>(audioFlags);
	retryAudio = false;

	if (!audioEngine->IsAudioDevicePresent()) {
		// no audio device found. Operating in silent mode.
		GameEngine::errorMessage(L"No Audio device found!", L"Audio Error");
		return false;
	}

	

	if (!game.initializeGame(this, hwnd, device, mouse)) {
		GameEngine::errorMessage(L"Game Manager failed to load.", L"Critical Failure");
		return false;
	}

	gameInitialized = true;

	return true;
}

void GameEngine::onAudioDeviceChange() {
	retryAudio = true;
}

void GameEngine::reloadGraphicsAssets() {
	game.reloadGraphicsAssets();
	
}




bool warningCanceled = false;
void GameEngine::run(double deltaTime) {


	update(deltaTime);
	render();

	if (!audioEngine->IsAudioDevicePresent() && !warningCanceled) {
		// no audio device found. Operating in silent mode.
		showWarningDialog(L"No audio device found. Operating in Silent Mode.\nEnd Message...",
			L"Audio Engine failure");
		warningCanceled = true;
	}

	if (retryAudio) {
		retryAudio = false;
		if (audioEngine->Reset()) {
			// restart looped sounds
		}
	} else if (!audioEngine->Update()) {
		if (audioEngine->IsCriticalError()) {
			showWarningDialog(L"Audio device lost!", L"Audio Engine failure");
			retryAudio = true;
		}
	}
}


void GameEngine::update(double deltaTime) {

	mouse->saveMouseState();
	keys->saveKeyState();
	slotManager->updateGamePads();

	game.update(deltaTime);
}



void GameEngine::render() {

	deviceContext->ClearRenderTargetView(renderTargetView.Get(), Colors::PeachPuff);

	/*batch->Begin(SpriteSortMode_Deferred, NULL, NULL, NULL, NULL, NULL, camera->translationMatrix());
	{*/
	game.draw(batch.get());


	swapChain->Present(0, 0);
}

void GameEngine::suspend() {

	stopFullScreen();
	if (gameInitialized)
		game.pause();
	if (audioEngine != NULL)
		audioEngine->Suspend();
}

void GameEngine::resume() {

	setFullScreen(Globals::FULL_SCREEN);
	if (audioEngine != NULL)
		audioEngine->Resume();
}

void GameEngine::exit() {

	if (swapChain.Get() != NULL)
		swapChain->SetFullscreenState(false, NULL);
	DestroyWindow(hwnd);
}

void GameEngine::controllerRemoved(ControllerSocketNumber controllerSocket,
	PlayerSlotNumber slotNumber) {

	game.controllerRemoved(controllerSocket, slotNumber);
}

void GameEngine::newController(shared_ptr<Joystick> newStick) {

	game.newController(newStick);
}