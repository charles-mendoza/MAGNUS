#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "GL/glew.h"

#include "Magnus/Magnus.h"
#include "Magnus/Input/Windows.h"
#include "Magnus/Renderers/OpenGL.h"

#include "UnitTest.h"

HWND g_hWnd = NULL;

HWND CreateGameWindow();
HGLRC CreateOpenGLDeviceContext();

int main()
{
	g_hWnd = CreateGameWindow();
	HGLRC OpenGLContext = CreateOpenGLDeviceContext();
	RECT FrameBounds;
	GetClientRect(g_hWnd, &FrameBounds);

	Magnus::Rendering::OpenGL* pRenderer = new Magnus::Rendering::OpenGL();
	pRenderer->Init();

	Magnus::Skin* pSkin = new Magnus::Skin(pRenderer);
	pSkin->Load("DefaultSkin.png");

	Magnus::Controls::Canvas* pCanvas = new Magnus::Controls::Canvas(pSkin);
	pCanvas->SetSize(FrameBounds.right, FrameBounds.bottom);
	pCanvas->SetDrawBackground(true);
	pCanvas->SetBackgroundColor(Magnus::Color(255, 51, 51, 51));

	UnitTest* pUnit = new UnitTest(pCanvas);

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
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			pRenderer->Begin();
			pCanvas->RenderCanvas();
			pRenderer->End();
			SwapBuffers(GetDC(g_hWnd));
		}
	}
	
	delete pCanvas;
	delete pSkin;
	delete pRenderer;

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(OpenGLContext);

	return 0;
}

HWND CreateGameWindow()
{
	WNDCLASSW	wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = DefWindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = L"MAGNUSWindow";
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	RegisterClassW(&wc);
	HWND hWindow = CreateWindowExW((WS_EX_APPWINDOW | WS_EX_WINDOWEDGE), wc.lpszClassName, L"MAGNUS - OpenGL Sample", (WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN) & ~(WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME), -1, -1, 800, 600, NULL, NULL, GetModuleHandle(NULL), NULL);
	ShowWindow(hWindow, SW_SHOW);
	SetForegroundWindow(hWindow);
	SetFocus(hWindow);
	return hWindow;
}

HGLRC CreateOpenGLDeviceContext()
{
	PIXELFORMATDESCRIPTOR pfd = { 0 };
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 32;
	int pixelFormat = ChoosePixelFormat(GetDC(g_hWnd), &pfd);

	if (pixelFormat == 0)
	{
		FatalAppExit(NULL, TEXT("ChoosePixelFormat() failed!"));
	}

	SetPixelFormat(GetDC(g_hWnd), pixelFormat, &pfd);
	HGLRC OpenGLContext = wglCreateContext(GetDC(g_hWnd));
	wglMakeCurrent(GetDC(g_hWnd), OpenGLContext);
	RECT r;

	if (GetClientRect(g_hWnd, &r))
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(r.left, r.right, r.bottom, r.top, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glViewport(0, 0, r.right - r.left, r.bottom - r.top);
	}

	return OpenGLContext;
}