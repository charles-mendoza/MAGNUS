#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <d3d9.h>

#include "Magnus/Magnus.h"
#include "Magnus/Input/Windows.h"
#include "Magnus/Renderers/DirectX9.h"

#include "UnitTest.h"

HWND					g_hWnd = NULL;
IDirect3D9*				g_pD3D = NULL;
IDirect3DDevice9*		g_pD3DDevice = NULL;
D3DPRESENT_PARAMETERS	g_D3DParams;

HWND CreateGameWindow();
void CreateD3DDevice();

int main()
{
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	g_hWnd = CreateGameWindow();
	CreateD3DDevice();

	Magnus::Rendering::DirectX9* pRenderer = new Magnus::Rendering::DirectX9(g_pD3DDevice);

	Magnus::Skin* pSkin = new Magnus::Skin(pRenderer);
	pSkin->Load("DefaultSkin.png");

	Magnus::Controls::Canvas* pCanvas = new Magnus::Controls::Canvas(pSkin);
	pCanvas->SetDrawBackground(true);
	pCanvas->SetBackgroundColor(Magnus::Color(255, 51, 51, 51));

	UnitTest* Test = new UnitTest(pCanvas);

	Magnus::Input::Windows Input;
	Input.Init(pCanvas);

	MSG msg;

	while (true)
	{
		if (!IsWindowVisible(g_hWnd)) { break; }

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			Input.ProcessMessage(msg);

			if (msg.message == WM_QUIT) { break; }

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_pD3DDevice->BeginScene();
			g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1, 0);
			pRenderer->Begin();
			pCanvas->RenderCanvas();
			pRenderer->End();
			g_pD3DDevice->EndScene();
			g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
		}
	}

	delete pCanvas;
	delete pSkin;
	delete pRenderer;

	if (g_pD3DDevice)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if (g_pD3D)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	return 0;
}

HWND CreateGameWindow()
{
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = DefWindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = L"MAGNUSWindow";
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	RegisterClass(&wc);
	HWND hWnd = CreateWindowEx((WS_EX_APPWINDOW | WS_EX_WINDOWEDGE), wc.lpszClassName, L"MAGNUS - DirectX9 Sample", (WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN) & ~(WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME), -1, -1, 800, 600, NULL, NULL, wc.hInstance, NULL);
	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);
	return hWnd;
}

void CreateD3DDevice()
{
	ZeroMemory(&g_D3DParams, sizeof(g_D3DParams));
	RECT ClientRect;
	GetClientRect(g_hWnd, &ClientRect);
	g_D3DParams.Windowed = TRUE;
	g_D3DParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_D3DParams.BackBufferWidth = ClientRect.right;
	g_D3DParams.BackBufferHeight = ClientRect.bottom;
	g_D3DParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	g_D3DParams.BackBufferFormat = D3DFMT_X8R8G8B8;
	g_D3DParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	HRESULT hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &g_D3DParams, &g_pD3DDevice);

	if (FAILED(hr))
	{
		OutputDebugString(L"Couldn't create D3D Device for some reason!");
		return;
	}
}