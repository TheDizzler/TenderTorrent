#if !defined(WIN32_DIRECTXTK_H)
/*****************************************************************************
*	$Note:	Temporarily using DirectXTK and ComPtr to get everything to
			a working state.$
*	$Author: TDGillon $
*	$Notice: (C) Copyright 2023 by AtomosZ Industries. All rights reserved. $
*****************************************************************************/

#include "../../packages/DirectXTK-feb2023/Inc/SpriteBatch.h"
#include "../../packages/DirectXTK-feb2023/Inc/SimpleMath.h"
#include "../../packages/DirectXTK-feb2023/Inc/DDSTextureLoader.h"
#include <wrl/client.h> // for ComPtr

using namespace DirectX::SimpleMath;
using namespace Microsoft::WRL;

#include "AtomosZ_Platform.h"

// all these forward declarations are getting to be an eyesore
internal bool32 IsFullScreen();

global bool ReportError(HRESULT hr, char* failMessage, char* failTitle);
global void DebugMessage(char* message, char* title);
global void DebugMessage(WCHAR* message, WCHAR* title);
global void DebugMessage(char* message);
global void DebugMessage(WCHAR* message);


/// <summary>
/// Something to draw.
/// </summary>
struct DrawBatch
{
	ID3D11ShaderResourceView* texture;
	ID3D11Resource* resource;
	RECT sourceRect;
	float layerDepth;
	Vector2 origin;
	Color tint;
	Vector2 scale;
	DirectX::SpriteEffects spriteEffect;


	Vector2 position;
	float rotation;
};



struct DirectXTextureData
{
	ID3D11ShaderResourceView* texture;
	ID3D11Resource* resource;
};

#define MAX_TEXTURE_COUNT 256
#define MAX_DRAWBATCH_COUNT 1024

struct DrawBatchMemory
{
	uint32 drawBatchCount;
	DrawBatch drawBatches[MAX_DRAWBATCH_COUNT];
};


struct DirectXTKDevice
{
	HWND hwnd;

	DirectX::SpriteBatch* spriteBatch;
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11Debug> debugDevice;
	ComPtr<ID3D11DeviceContext> deviceContext;
	ComPtr<IDXGISwapChain> swapChain;
	ComPtr<ID3D11RenderTargetView> renderTargetView;
	ComPtr<ID3D11RasterizerState> rasterState;

	D3D11_VIEWPORT mainViewport;

	uint32 loadedTextureCount;
	DirectXTextureData loadedTextures[MAX_TEXTURE_COUNT];

	DrawBatchMemory* drawBatchMemory;

	float clientWidth, clientHeight;
};



internal DirectXTextureData* LoadTextureAsset(GameMemory* memory, char* textureFilename)
{
	DirectXTKDevice* dxtkDevice = (DirectXTKDevice*)memory->graphicsDevice;

	Assert(dxtkDevice->loadedTextureCount < MAX_TEXTURE_COUNT);

	DirectXTextureData* result = dxtkDevice->loadedTextures + dxtkDevice->loadedTextureCount++;

	if (ReportError(
		DirectX::CreateDDSTextureFromFile(dxtkDevice->device.Get(), ConvertCharStarToWCharT(textureFilename),
			&result->resource, &result->texture), textureFilename, "Failed to load texture"))
		return 0;

	return result;
}

#include "AtomosZEngine.h"

inline void PushBatch(GameMemory* gameMemory, DrawBatch batch)
{
	TransientState* transState = (TransientState*)gameMemory->transientStorage;
	DrawBatchMemory* drawBatchMem = (DrawBatchMemory*)transState->drawBatchMemory;
	Assert(drawBatchMem->drawBatchCount < MAX_DRAWBATCH_COUNT);
	DrawBatch* batchMem = drawBatchMem->drawBatches + drawBatchMem->drawBatchCount++;
	//ZeroStruct(batchMem);
	*batchMem = batch;
}


inline void DirectXTKRender(GameMemory* gameMemory)
{
	DirectXTKDevice* dxtkDevice = (DirectXTKDevice*)gameMemory->graphicsDevice;
	dxtkDevice->deviceContext->ClearRenderTargetView(dxtkDevice->renderTargetView.Get(), DirectX::Colors::PeachPuff);

	DirectX::SpriteBatch* spriteBatch = dxtkDevice->spriteBatch;
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred);

	TransientState* transState = (TransientState*)gameMemory->transientStorage;
	DrawBatchMemory* drawBatchMem = (DrawBatchMemory*)transState->drawBatchMemory;

	
	for (uint32 i = 0; i < drawBatchMem->drawBatchCount; ++i)
	{
		DrawBatch* batch = (DrawBatch*)drawBatchMem->drawBatches + i;
		Assert(batch);
		spriteBatch->Draw(batch->texture, batch->position,
			&batch->sourceRect, batch->tint, batch->rotation,
			batch->origin, batch->scale, batch->spriteEffect, batch->layerDepth);

		 // @NOTE: zeroing here might be bad? If the data is copied to the spritebatch/graphics card
		 //		then it's NBD and we're GTG but if it's still being referenced, we're screwed.
		 //		It's probably better to do this when pushing the draw batch.
		//ZeroStruct(batch); // yeah, you know what? Moving this PushBatch().
	}
	drawBatchMem->drawBatchCount = 0;

	spriteBatch->End();
	dxtkDevice->swapChain->Present(0, 0);
}




inline void InitializeRenderTarget(DirectXTKDevice* dxtkDevice)
{
	/** **** Create our Render Target **** **/
	ComPtr<ID3D11Texture2D> backBufferPtr;
	if (ReportError(
		dxtkDevice->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)backBufferPtr.GetAddressOf()),
		"Could not get pointer to back buffer.", "ERROR"))
	{
		FatalError;
	}


	if (ReportError(
		dxtkDevice->device->CreateRenderTargetView(backBufferPtr.Get(),
			NULL, dxtkDevice->renderTargetView.GetAddressOf()),
		"Could not create render target view.", "ERROR"))
	{
		FatalError;
	}


	dxtkDevice->deviceContext->OMSetRenderTargets(1, dxtkDevice->renderTargetView.GetAddressOf(), nullptr);
}


inline void InitializeAndSetViewport(DirectXTKDevice* dxtkDevice)
{
	/** **** Create D3D Viewport **** **/
	D3D11_VIEWPORT mainViewport;
	ZeroMemory(&mainViewport, sizeof(D3D11_VIEWPORT));
	mainViewport.TopLeftX = 0;
	mainViewport.TopLeftY = 0;
	mainViewport.Width = dxtkDevice->clientWidth;
	mainViewport.Height = dxtkDevice->clientHeight;
	mainViewport.MinDepth = 0.0f;
	mainViewport.MaxDepth = 1.0f;
	dxtkDevice->deviceContext->RSSetViewports(1, &mainViewport);

	dxtkDevice->spriteBatch->SetViewport(mainViewport);


	/*scissorRECTs[0].left = Globals::MAIN_VIEWPORT_LEFT;
	scissorRECTs[0].right = Globals::MAIN_VIEWPORT_WIDTH;
	scissorRECTs[0].top = Globals::MAIN_VIEWPORT_TOP;
	scissorRECTs[0].bottom = Globals::MAIN_VIEWPORT_HEIGHT;

	deviceContext->RSSetScissorRects(1, scissorRECTs);*/

	/*ZeroMemory(&altViewport, sizeof(D3D11_VIEWPORT));
	altViewport.TopLeftX = 0;
	altViewport.TopLeftY = 0;
	altViewport.Width = Globals::WINDOW_WIDTH;
	altViewport.Height = Globals::WINDOW_HEIGHT;
	altViewport.MinDepth = 0.0f;
	altViewport.MaxDepth = 1.0f;*/

}


struct Display
{
	ComPtr<IDXGIOutput> output;
	uint32 modeCount;
	DXGI_MODE_DESC* modeList;
};

struct AdapterDetails
{
	ComPtr<IDXGIAdapter> adapter;
	int32 displayCount;
	Display* displays;
};

struct AdapterList
{
	uint32 adapterCount;
	// Pointer to array in Transient Storage.
	//ComPtr<IDXGIAdapter>* adapters;
	AdapterDetails* adapters;
};

/// <summary>
/// Stores found adapters in transient storage and AdapterList.
/// </summary>
/// <param name="transState"></param>
/// <returns></returns>
inline AdapterList GetDisplayAdapters(TransientState* transState)
{
	AdapterList result = {};
	result.adapterCount = 0;

	ComPtr<IDXGIFactory1> factory;
	// Create a DirectX graphics interface factory.
	if (ReportError(
		CreateDXGIFactory1(
			__uuidof(IDXGIFactory), (void**)factory.GetAddressOf()),
		"Cannot create DXGI factory.", "ERROR"))
	{
		DebugMessage("Could not create DXGIFactory. Bad times, bad times.", "Fatal Error");
		return result;
	}


	result.adapters = PushStruct(&transState->transArena, AdapterDetails);
	ComPtr<IDXGIAdapter> adapter;


	if (factory->EnumAdapters(result.adapterCount++, adapter.GetAddressOf()) == DXGI_ERROR_NOT_FOUND)
	{
		DebugMessage("Failed to get an adpater. Bad times, bad times.", "No adapters found");
		return result;
	}
	result.adapters->adapter = adapter;

   // get all adapters (gfx cards)
	while (factory->EnumAdapters(result.adapterCount++, adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND)
	{
		//ComPtr<IDXGIAdapter>* nextAdapter = PushStruct(&transState->transArena, ComPtr<IDXGIAdapter>);
		AdapterDetails* adapterDetails = PushStruct(&transState->transArena, AdapterDetails);
		adapterDetails->adapter = adapter;

		DXGI_ADAPTER_DESC desc;
		ZeroMemory(&desc, sizeof(DXGI_ADAPTER_DESC));
		adapter->GetDesc(&desc);

		DebugMessage(desc.Description);
	}

	--result.adapterCount;

	return result;
}

inline void DEBUGPrintOutputDescription(IDXGIOutput* adapterOutput, DXGI_ADAPTER_DESC adapterDesc)
{
	DXGI_OUTPUT_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_OUTPUT_DESC));
	adapterOutput->GetDesc(&desc);

	wchar_t buffer[512];
	swprintf_s(buffer, 512, L"Adapter: %s\n\tDisplay: %s\n\tAttachedToDesktop: %s\n",
		adapterDesc.Description, desc.DeviceName, desc.AttachedToDesktop ? L"true" : L"false");
	DebugMessage(buffer);
}

struct DefaultOutputDevice
{
	int32 defaultAdapter;
	int32 defaultDisplayIndex;
};

/// <summary>
/// Returns index of a display to use as default.
/// </summary>
/// <param name="transState"></param>
/// <param name="adapterList"></param>
/// <returns></returns>
inline DefaultOutputDevice GetDisplays(TransientState* transState, AdapterList* adapterList)
{
	DefaultOutputDevice result = {};
	result.defaultAdapter = -1;
	result.defaultDisplayIndex = -1;

	for (uint32 i = 0; i < adapterList->adapterCount; ++i)
	{
		AdapterDetails* adapterDetail = adapterList->adapters + i;
		adapterDetail->displayCount = 0;

		DXGI_ADAPTER_DESC adapterDesc;
		ZeroMemory(&adapterDesc, sizeof(DXGI_ADAPTER_DESC));
		adapterDetail->adapter->GetDesc(&adapterDesc);
		ComPtr<IDXGIOutput> adapterOutput;

		if (adapterDetail->adapter->EnumOutputs(
			adapterDetail->displayCount++, adapterOutput.GetAddressOf()) == DXGI_ERROR_NOT_FOUND)
		{
			continue; // there are no outputs connected to this adapter
		}

		adapterDetail->displays = PushStruct(&transState->transArena, Display);
		adapterDetail->displays->output = adapterOutput;

		DEBUGPrintOutputDescription(adapterOutput.Get(), adapterDesc);


		while (adapterDetail->adapter->EnumOutputs(
			adapterDetail->displayCount, adapterOutput.GetAddressOf()) != DXGI_ERROR_NOT_FOUND)
		{
			Display* display = PushStruct(&transState->transArena, Display);

			Display* newDisplay = (adapterDetail->displays + adapterDetail->displayCount);
			newDisplay = display;
			newDisplay->output = adapterOutput;

			DEBUGPrintOutputDescription(newDisplay->output.Get(), adapterDesc);


			++adapterDetail->displayCount;
		}

		--adapterDetail->displayCount;

		if (adapterDetail->displayCount > -1 && result.defaultAdapter == -1)
		{
			result.defaultAdapter = i;
			result.defaultDisplayIndex = 0;
		}
	}

	return result;
}

inline void PopulateDisplayModeList(TransientState* transState, Display* display)
{
	// Find total modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format
	if (ReportError(
		display->output->GetDisplayModeList(
			DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
			&display->modeCount, NULL), "Error enumerating display modes.", "ERROR"))
	{
		FatalError;
	}

	display->modeList = PushArray(&transState->transArena, display->modeCount, DXGI_MODE_DESC);

	if (ReportError(
		display->output->GetDisplayModeList(
			DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
			&display->modeCount, display->modeList), "Error getting display mode list.", "ERROR"))
	{
		FatalError;
	}
}


/// <summary>
/// A debug function to make sure we're using the correct graphics adapter.
/// </summary>
/// <param name="selectedIndex"></param>
inline void VerifyAdapter(ComPtr<ID3D11Device> deviceCheck)
{
	IDXGIDevice* dxgiDev;
	if (ReportError(
		deviceCheck.Get()->QueryInterface(
			__uuidof(IDXGIDevice), (void**)&dxgiDev),
		"Error querying device interface.", "ERROR"))
	{
		FatalError;
	}

	IDXGIAdapter* dxgiAdapter;
	if (ReportError(dxgiDev->GetAdapter(&dxgiAdapter),
		"Error getting idxgi adapter from dxgi device.", "ERROR"))
	{
		FatalError;
	}


	IDXGIFactory* factory;
	if (ReportError(dxgiAdapter->GetParent(
		__uuidof(IDXGIFactory), (void**)&factory),
		"Error getting factory from idxgi adapter.", "ERROR"))
	{
		FatalError;
	}


	DXGI_ADAPTER_DESC adapterDesc;
	ZeroMemory(&adapterDesc, sizeof(DXGI_ADAPTER_DESC));
	dxgiAdapter->GetDesc(&adapterDesc);

	wchar_t buffer[512];
	swprintf_s(buffer, 512, L"Verified adapter: %s", adapterDesc.Description);
	DebugMessage(buffer);

	dxgiDev->Release();
	dxgiAdapter->Release();
	factory->Release();
}

inline void InitializeAdapter(TransientState* transState, Display* display,
	DirectXTKDevice* dxtkDevice, ComPtr<IDXGIAdapter> adapter, WindowDimension windowDimension)
{
	PopulateDisplayModeList(transState, display);

		//SetDisplayMode();
	uint32 selectedDisplayModeIndex = 0;
	bool32 foundDisplayMode = false;
	for (uint32 i = 0; i < display->modeCount; ++i)
	{
		DXGI_MODE_DESC* modeDesc = display->modeList + i;

		char mbuffer[256];
		_snprintf_s(mbuffer, sizeof(mbuffer),
			"modeDesc.width: %d  modeDesc.height: %d\n",
			modeDesc->Width, modeDesc->Height);
		OutputDebugStringA(mbuffer);

		if (modeDesc->Height == windowDimension.height
			&& modeDesc->Width == windowDimension.width)
		{
			selectedDisplayModeIndex = i;
			foundDisplayMode = true;
			break;
		}
	}

	// @TODO: must automatically pick displaymode then adjust
	if (!foundDisplayMode)
	{
		FatalError;
	}

	DXGI_MODE_DESC selectedDisplayMode = display->modeList[selectedDisplayModeIndex];

	dxtkDevice->clientWidth = (float)selectedDisplayMode.Width;
	dxtkDevice->clientHeight = (float)selectedDisplayMode.Height;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	UINT bufferCount = 1;
// Use Alt-Enter to switch between full screen and windowed mode.;
	UINT swapChainFlags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	swapChainDesc.BufferCount = bufferCount; // Back buffer count
	swapChainDesc.BufferDesc = selectedDisplayMode;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = dxtkDevice->hwnd;
	swapChainDesc.SampleDesc.Count = 4; // how many multisamples
										// 1 = turn multisampling off.
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = !IsFullScreen();
	swapChainDesc.Flags = swapChainFlags;

	UINT createDeviceFlags = 0;
#ifdef ATOMOSZ_INTERNAL
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			//D3D_FEATURE_LEVEL_10_1,
			//D3D_FEATURE_LEVEL_10_0,
	};

	ComPtr<IDXGISwapChain> swapChain;
	D3D_FEATURE_LEVEL featureLevel;

	if (ReportError(
		D3D11CreateDeviceAndSwapChain(adapter.Get(),
			D3D_DRIVER_TYPE_UNKNOWN, NULL, createDeviceFlags, featureLevels,
			_countof(featureLevels), D3D11_SDK_VERSION, &swapChainDesc,
			swapChain.GetAddressOf(), dxtkDevice->device.GetAddressOf(), &featureLevel,
			dxtkDevice->deviceContext.GetAddressOf()),
		"Error creating Device and Swap Chain.", "ERROR"))
	{
		FatalError;
	}

	dxtkDevice->swapChain = swapChain;

#ifdef ATOMOSZ_INTERNAL
	dxtkDevice->device.Get()->QueryInterface(__uuidof(ID3D11Debug),
		reinterpret_cast<void**>(dxtkDevice->debugDevice.GetAddressOf()));
#endif

	VerifyAdapter(dxtkDevice->device);

	CD3D11_RASTERIZER_DESC rsDesc(D3D11_FILL_SOLID, D3D11_CULL_BACK, FALSE,
		0, 0.f, 0.f, TRUE, TRUE, TRUE, FALSE);
	if (ReportError(
		dxtkDevice->device->CreateRasterizerState(&rsDesc, dxtkDevice->rasterState.GetAddressOf()),
		"Error creating RasterizerState.", "ERROR"))
	{
		FatalError;
	}
	}

inline void InitializeViewport(DirectXTKDevice* dxtkDevice)
{
	/** **** Create D3D Viewport **** **/
	ZeroMemory(&dxtkDevice->mainViewport, sizeof(D3D11_VIEWPORT));
	dxtkDevice->mainViewport.TopLeftX = 0;
	dxtkDevice->mainViewport.TopLeftY = 0;
	dxtkDevice->mainViewport.Width = (float)dxtkDevice->clientWidth;
	dxtkDevice->mainViewport.Height = (float)dxtkDevice->clientHeight;
	dxtkDevice->mainViewport.MinDepth = 0.0f;
	dxtkDevice->mainViewport.MaxDepth = 1.0f;

	dxtkDevice->deviceContext->RSSetViewports(1, &dxtkDevice->mainViewport);
}


inline void InitializeSpriteBatch(GameMemory* memory, WindowDimension windowDimension, HWND hwnd)
{
	Assert(sizeof(TransientState) <= memory->transientStorageSize);
	TransientState* transState = (TransientState*)memory->transientStorage;

	InitializeMemoryArena(&transState->transArena,
		memory->transientStorageSize - sizeof(TransientState),
		(uint8*)memory->transientStorage + sizeof(TransientState));

	DrawBatchMemory* drawBatchMemory = PushStruct(&transState->transArena, DrawBatchMemory);
	drawBatchMemory->drawBatchCount = 0;
	transState->drawBatchMemory = drawBatchMemory;


	GameState* gameState = (GameState*)memory->permanentStorage;
	DirectXTKDevice* dxtkDevice = PushStruct(&gameState->memoryArena, DirectXTKDevice);
	dxtkDevice->loadedTextureCount = 0;
	dxtkDevice->hwnd = hwnd;

	memory->graphicsDevice = dxtkDevice;

	AdapterList adapterList = GetDisplayAdapters(transState);
	if (adapterList.adapterCount == 0)
	{
		FatalError; // TODO: A friendly way to tell the user their PC is f'd.
	}

	DefaultOutputDevice defaultOutput = GetDisplays(transState, &adapterList);
	if (defaultOutput.defaultAdapter == -1)
	{
		FatalError; // TODO: A friendly way to tell the user their PC is f'd.
	}


	// TODO: get the most "natural" output instead of the the first one?
	AdapterDetails selectedAdapter = adapterList.adapters[defaultOutput.defaultAdapter];
	Display display = adapterList.adapters->displays[defaultOutput.defaultDisplayIndex];

	//TransientState* transState, Display* display,
		//DirectXTKDevice* , AdapterDetails* selectedAdapter, WindowDimension )
	InitializeAdapter(transState, &display, dxtkDevice, selectedAdapter.adapter, windowDimension);
	InitializeRenderTarget(dxtkDevice);
	InitializeViewport(dxtkDevice);

	dxtkDevice->spriteBatch = new DirectX::SpriteBatch(dxtkDevice->deviceContext.Get());
	InitializeAndSetViewport(dxtkDevice);
}






inline void GetTextureDimensions(ID3D11Resource* res, uint32* width, uint32* height)
{
	D3D11_RESOURCE_DIMENSION dim;
	res->GetType(&dim);

	switch (dim)
	{
		case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
		{
			auto texture = reinterpret_cast<ID3D11Texture2D*>(res);
			D3D11_TEXTURE2D_DESC desc;
			texture->GetDesc(&desc);
			if (width)
				*width = desc.Width; // width in pixels
			if (height)
				*height = desc.Height; // height in pixels
		} break;

		default:

			if (width)
				*width = 0; // width in pixels
			if (height)
				*height = 0; // height in pixels
	}
}

#define WIN32_DIRECTXTK_H
#endif