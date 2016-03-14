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


	game.reset(new GameManager(this));
	if (!game->initializeGame(device, mouse.get()))
		return false;


	return true;
}






void GameEngine::run(double deltaTime, int fps) {

	detectInput(deltaTime);

	//waitingForInput = false;
	/*if (GetKeyState(VK_LBUTTON) & 0x8000)
		waitingForInput = false;*/

		update(deltaTime);
		render(deltaTime);

}


void GameEngine::detectInput(double deltaTime) {


	inputKB->Acquire();
	inputMouse->Acquire();

	inputMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID) &mouse->setCurrentState());
	inputKB->GetDeviceState(sizeof(keyboardState), (LPVOID) &keyboardState);

	POINT cursorPos;
	GetCursorPos(&cursorPos);

	mouse->setPosition(Vector2(cursorPos.x, cursorPos.y));
}


void GameEngine::update(double deltaTime) {

	game->update(deltaTime, keyboardState, mouse.get());
}



void GameEngine::render(double deltaTime) {


	deviceContext->ClearRenderTargetView(renderTargetView, Colors::CornflowerBlue);

	batch->Begin(SpriteSortMode_Deferred);
	{
		game->draw(batch.get());
		mouse->draw(batch.get());
	}
	batch->End();


	swapChain->Present(0, 0);
}

void GameEngine::exit() {

	/*if (MessageBox(0, L"Are you sure you want to exit?",
		L"Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)*/
		DestroyWindow(hwnd);
}

