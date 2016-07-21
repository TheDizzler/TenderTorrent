#include "GraphicsEngine.h"
#include <SimpleMath.h>
#include <DirectXColors.h>




GraphicsEngine::GraphicsEngine() {
	
}




GraphicsEngine::~GraphicsEngine() {


	if (renderTargetView)
		renderTargetView->Release();
	if (swapChain)
		swapChain->Release();
	if (deviceContext)
		deviceContext->Release();
	if (device)
		device->Release();

}



bool GraphicsEngine::initD3D(HWND hwnd) {

	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};

	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3

	};

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);


	/** **** Create SWAP CHAIN **** **/
	DXGI_MODE_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = Globals::WINDOW_WIDTH;
	bufferDesc.Height = Globals::WINDOW_HEIGHT;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;


	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // alt-enter fullscreen

	HRESULT hr;

	
	for (int i = 0; i < numDriverTypes; ++i) {

		hr = D3D11CreateDeviceAndSwapChain(NULL, driverTypes[i], NULL, createDeviceFlags,
			featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device,
			&featureLevel, &deviceContext);

		if (SUCCEEDED(hr)) {

			driverType = driverTypes[i];
			break;
		}
	}

	if (FAILED(hr)) {
		OutputDebugString(L"Failed to create Swap Chain");
		return false;
	}


	/** **** Create our Render Target **** **/
	ID3D11Texture2D* backBufferTexture = 0;
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**) &backBufferTexture)))
		return false;


	if (FAILED(device->CreateRenderTargetView(backBufferTexture, nullptr, &renderTargetView)))
		return false;

	deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);

	backBufferTexture->Release();


	/** **** Create Viewport **** **/
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(Globals::WINDOW_WIDTH);
	viewport.Height = static_cast<float>(Globals::WINDOW_HEIGHT);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	deviceContext->RSSetViewports(1, &viewport);

	batch.reset(new SpriteBatch(deviceContext));


	return true;

}
