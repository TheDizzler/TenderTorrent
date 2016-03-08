#include "GameEngine.h"


GameEngine::GameEngine() {

}


GameEngine::~GameEngine() {


}


bool GameEngine::initEngine(HWND hw, HINSTANCE hInstance) {

	hwnd = hw;

	if (!initD3D(hwnd)) {
		MessageBox(0, L"Direct3D Initialization Failed", L"Error", MB_OK);
		return false;
	}

	if (!initDirectInput(hInstance, hwnd)) {
		MessageBox(0, L"Input Init failed", L"Error", MB_OK);
		return false;
	}

	if (!initStage()) {
		MessageBox(0, L"Stage Initialization Failed", L"Error", MB_OK);
		return false;
	}

	SetCursorPos(200, 200);

	return true;
}

bool GameEngine::initStage() {


	levelManager.reset(new LevelManager(device));
	if (!levelManager->initStage(device))
		return false;


	return true;
}






void GameEngine::run(double deltaTime, int fps) {

	detectInput(deltaTime);

	if (keyboardState[DIK_ESCAPE]) {
		waitingForInput = true;
		if (MessageBox(0, L"Are you sure you want to exit?",
			L"Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)
			DestroyWindow(hwnd);

	}
	//waitingForInput = false;
	if (GetKeyState(VK_LBUTTON) & 0x8000)
		waitingForInput = false;
	if (!waitingForInput) {
		update(deltaTime);
		render(deltaTime);
	} else {
		batch->Begin(SpriteSortMode_Deferred);
		levelManager->pauseScreen(deltaTime, batch.get());
		batch->End();
		swapChain->Present(0, 0);
	}
}


void GameEngine::detectInput(double deltaTime) {


	inputKB->Acquire();
	inputMouse->Acquire();


	inputMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID) &mouseCurrentState);
	inputKB->GetDeviceState(sizeof(keyboardState), (LPVOID) &keyboardState);


	GetCursorPos(&cursorPos);

}

void GameEngine::update(double deltaTime) {

	levelManager->update(deltaTime, keyboardState, mouseCurrentState, Vector2(cursorPos.x, cursorPos.y));
}



void GameEngine::render(double deltaTime) {


	

	deviceContext->ClearRenderTargetView(renderTargetView, Colors::CornflowerBlue);

	batch->Begin(SpriteSortMode_Deferred);
	{
		
		levelManager->draw(batch.get());

	}
	batch->End();


	swapChain->Present(0, 0);
}

