#include "stdafx.h"

// General
#include "OpenGLAdapter_Native.h"

CInput* input;

#pragma region OpenGL

typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext, const int *attribList);
wglCreateContextAttribsARB_type *wglCreateContextAttribsARB;

#define GL_FALSE                          0
#define GL_TRUE                           1

// See https://www.opengl.org/registry/specs/ARB/wgl_create_context.txt for all values
#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001

typedef BOOL WINAPI wglChoosePixelFormatARB_type(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
wglChoosePixelFormatARB_type *wglChoosePixelFormatARB;

// See https://www.opengl.org/registry/specs/ARB/wgl_pixel_format.txt for all values
#define WGL_DRAW_TO_WINDOW_ARB                    0x2001
#define WGL_ACCELERATION_ARB                      0x2003
#define WGL_SUPPORT_OPENGL_ARB                    0x2010
#define WGL_DOUBLE_BUFFER_ARB                     0x2011
#define WGL_PIXEL_TYPE_ARB                        0x2013
#define WGL_COLOR_BITS_ARB                        0x2014
#define WGL_DEPTH_BITS_ARB                        0x2022
#define WGL_STENCIL_BITS_ARB                      0x2023

#define WGL_FULL_ACCELERATION_ARB                 0x2027
#define WGL_TYPE_RGBA_ARB                         0x202B

#pragma endregion

static void init_opengl_extensions(void)
{
	WNDCLASS renderWindowClass;
	ZeroMemory(&renderWindowClass, sizeof(WNDCLASS));
	renderWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	renderWindowClass.lpfnWndProc = &DefWindowProcA;
	renderWindowClass.hInstance = GetModuleHandle(0);
	renderWindowClass.lpszMenuName = NULL;
	renderWindowClass.lpszClassName = "TestRenderClass";

	if (!RegisterClassA(&renderWindowClass)) {
		fail1("Failed to register dummy OpenGL window.");
	}

	HWND dummy_window = CreateWindowExA(
		0,
		renderWindowClass.lpszClassName,
		"Dummy OpenGL Window",
		0,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		renderWindowClass.hInstance,
		0);

	if (!dummy_window) {
		fail1("Failed to create dummy OpenGL window.");
	}

	HDC dummy_dc = GetDC(dummy_window);

	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.cColorBits = 32;
	pfd.cAlphaBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;


	int pixel_format = ChoosePixelFormat(dummy_dc, &pfd);
	if (!pixel_format) {
		fail1("Failed to find a suitable pixel format.");
	}
	if (!SetPixelFormat(dummy_dc, pixel_format, &pfd)) {
		fail1("Failed to set the pixel format.");
	}

	HGLRC dummy_context = wglCreateContext(dummy_dc);
	if (!dummy_context)
	{
		fail1("Failed to create a dummy OpenGL rendering context.");
	}

	if (!wglMakeCurrent(dummy_dc, dummy_context))
	{
		fail1("Failed to activate dummy OpenGL rendering context.");
	}

	wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type*)wglGetProcAddress(
		"wglCreateContextAttribsARB");
	wglChoosePixelFormatARB = (wglChoosePixelFormatARB_type*)wglGetProcAddress(
		"wglChoosePixelFormatARB");

	wglMakeCurrent(dummy_dc, 0);
	wglDeleteContext(dummy_context);
	ReleaseDC(dummy_window, dummy_dc);
	DestroyWindow(dummy_window);
}

static HGLRC init_opengl(HDC real_dc)
{
	init_opengl_extensions();

	// Now we can choose a pixel format the modern way, using wglChoosePixelFormatARB.
	int pixel_format_attribs[] = {
		WGL_DRAW_TO_WINDOW_ARB,     GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB,     GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB,      GL_TRUE,
		WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
		WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB,         32,
		WGL_DEPTH_BITS_ARB,         24,
		WGL_STENCIL_BITS_ARB,       8,
		0
	};

	int pixel_format;
	UINT num_formats;
	wglChoosePixelFormatARB(real_dc, pixel_format_attribs, 0, 1, &pixel_format, &num_formats);
	if (!num_formats) {
		fail1("Failed to set the OpenGL 3.3 pixel format.");
	}

	PIXELFORMATDESCRIPTOR pfd;
	DescribePixelFormat(real_dc, pixel_format, sizeof(pfd), &pfd);
	if (!SetPixelFormat(real_dc, pixel_format, &pfd)) {
		fail1("Failed to set the OpenGL 3.3 pixel format.");
	}

	// Specify that we want to create an OpenGL 3.3 core profile context
	int gl33_attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 4,
		WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0,
	};

	HGLRC gl33_context = wglCreateContextAttribsARB(real_dc, 0, gl33_attribs);
	if (!gl33_context) 
	{
		fail1("Failed to create OpenGL 3.3 context.");
	}

	if (!wglMakeCurrent(real_dc, gl33_context)) 
	{
		fail1("Failed to activate OpenGL 3.3 rendering context.");
	}

	return gl33_context;
}


// Convert the message ID into a MouseButton ID
static int32 DecodeMouseButton(UINT messageID)
{
	int32 mouseButton = -1;
	switch (messageID)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	{
		mouseButton = OW_MOUSE_BUTTON_LEFT;
	}
	break;
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
	{
		mouseButton = OW_MOUSE_BUTTON_RIGHT;
	}
	break;
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
	{
		mouseButton = OW_MOUSE_BUTTON_MIDDLE;
	}
	break;
	}

	return mouseButton;
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT paintStruct;
		HDC hDC = BeginPaint(hwnd, &paintStruct);
		EndPaint(hwnd, &paintStruct);
	}
	break;
	case WM_MOUSEMOVE:
	{
		bool lButton = (wParam & MK_LBUTTON) != 0;
		bool rButton = (wParam & MK_RBUTTON) != 0;
		bool mButton = (wParam & MK_MBUTTON) != 0;
		bool shift = (wParam & MK_SHIFT) != 0;
		bool control = (wParam & MK_CONTROL) != 0;

		int x = ((int)(short)LOWORD(lParam));
		int y = ((int)(short)HIWORD(lParam));

		//MouseMotionEventArgs mouseMotionEventArgs(*gs_WindowHandle, lButton, mButton, rButton, control, shift, x, y);
		//gs_WindowHandle->OnMouseMoved(mouseMotionEventArgs);
		input->MousePositionCallback(vec2(x, y));
	}
	break;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	{
		bool lButton = (wParam & MK_LBUTTON) != 0;
		bool rButton = (wParam & MK_RBUTTON) != 0;
		bool mButton = (wParam & MK_MBUTTON) != 0;
		bool shift = (wParam & MK_SHIFT) != 0;
		bool control = (wParam & MK_CONTROL) != 0;

		int x = ((int)(short)LOWORD(lParam));
		int y = ((int)(short)HIWORD(lParam));

		//MouseButtonEventArgs mouseButtonEventArgs(*gs_WindowHandle, DecodeMouseButton(message), MouseButtonEventArgs::Pressed, lButton, mButton, rButton, control, shift, x, y);
		//gs_WindowHandle->OnMouseButtonPressed(mouseButtonEventArgs);
		
		input->MouseCallback(DecodeMouseButton(message), OW_PRESS, 0);
	}
	break;

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	{
		bool lButton = (wParam & MK_LBUTTON) != 0;
		bool rButton = (wParam & MK_RBUTTON) != 0;
		bool mButton = (wParam & MK_MBUTTON) != 0;
		bool shift = (wParam & MK_SHIFT) != 0;
		bool control = (wParam & MK_CONTROL) != 0;

		int x = ((int)(short)LOWORD(lParam));
		int y = ((int)(short)HIWORD(lParam));

		//MouseButtonEventArgs mouseButtonEventArgs(*gs_WindowHandle, DecodeMouseButton(message), MouseButtonEventArgs::Released, lButton, mButton, rButton, control, shift, x, y);
		//gs_WindowHandle->OnMouseButtonReleased(mouseButtonEventArgs);

		input->MouseCallback(DecodeMouseButton(message), OW_RELEASE, 0);
	}
	break;

	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		result = DefWindowProc(hwnd, message, wParam, lParam);
		break;
	}

	return result;
}

#define RENDER_WINDOW_CLASS_NAME "RenderWindowClass"

static HWND create_window()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	HINSTANCE hDll;
	hDll = LoadLibrary("SHELL32.dll");

	// Register a window class for creating our render windows with.
	WNDCLASSEX renderWindowClass;
	renderWindowClass.cbSize = sizeof(WNDCLASSEX);
	renderWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	renderWindowClass.lpfnWndProc = &WndProc;
	renderWindowClass.cbClsExtra = 0;
	renderWindowClass.cbWndExtra = 0;
	renderWindowClass.hInstance = hInstance;
	renderWindowClass.hIcon = LoadIcon(hDll, MAKEINTRESOURCE(2));
	renderWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	renderWindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	renderWindowClass.lpszMenuName = NULL;
	renderWindowClass.lpszClassName = RENDER_WINDOW_CLASS_NAME;
	renderWindowClass.hIconSm = LoadIcon(hDll, MAKEINTRESOURCE(2));

	if (!RegisterClassEx(&renderWindowClass))
	{
		fail1("Failed to register the render window class.");
	}

	// Specify a desired width and height, then adjust the rect so the window's client area will be
	// that size.
	RECT rect;
	ZeroMemory(&rect, sizeof(RECT));
	rect.right = 1280;
	rect.bottom = 1024;

	DWORD window_style = WS_OVERLAPPEDWINDOW;
	AdjustWindowRect(&rect, window_style, false);

	HWND hWindow = CreateWindowExA(
		0,
		renderWindowClass.lpszClassName,
		"OpenGL",
		window_style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWindow) 
	{
		fail1("Failed to create window.");
	}

	ShowWindow(hWindow, 1);

	return hWindow;
}

OpenGLAdapter_Native::OpenGLAdapter_Native()
{
	CGroupVideo& groupVideo = GetSettingsGroup<CGroupVideo>();
	CGroupOpenGL& openglSettings = GetSettingsGroup<CGroupOpenGL>();

	m_hwnd = create_window();
	m_gldc = GetDC(m_hwnd);
	HGLRC glrc = init_opengl(m_gldc);

	m_Input = new CInput();
	input = m_Input;

	m_timer.setEnabled(true);
}

OpenGLAdapter_Native::~OpenGLAdapter_Native()
{
}

bool OpenGLAdapter_Native::SwapWindowBuffers()
{
	MSG msg = { };
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			//EventArgs eventArgs(*this);
			//OnExit(eventArgs);
			return false;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	SwapBuffers(m_gldc);

	return true;
}

double OpenGLAdapter_Native::GetTime()
{
	return m_timer.getElapsedTimeMS() * 1000.0f;
}

CInput * OpenGLAdapter_Native::GetInput()
{
	return m_Input;
}

void OpenGLAdapter_Native::MakeMainContext()
{
}

void OpenGLAdapter_Native::MakeThreadContext()
{
}

void OpenGLAdapter_Native::SetWindowSize(int32 _width, int32 _height)
{
}

void OpenGLAdapter_Native::SetWindowTitle(cstring _title)
{
}

void OpenGLAdapter_Native::CreateCursor()
{
}

void OpenGLAdapter_Native::ShowCursor()
{
}

void OpenGLAdapter_Native::HideCursor()
{
}

void OpenGLAdapter_Native::setMousePosition(cvec2 _mousePosition)
{
	RECT rc;
	GetClientRect(m_hwnd, &rc); // get client coords
	ClientToScreen(m_hwnd, reinterpret_cast<POINT*>(&rc.left)); // convert top-left
	ClientToScreen(m_hwnd, reinterpret_cast<POINT*>(&rc.right)); // convert bottom-right

	SetCursorPos(rc.left + _mousePosition.x, rc.top + _mousePosition.y);
}

int OpenGLAdapter_Native::isExtensionSupported(const char * _extension)
{
	return 0;
}

void* OpenGLAdapter_Native::getProcAddress(const char * _procname)
{
	void* p = (void*) wglGetProcAddress(_procname);
	if (p == (void*) 0x0 ||
		p == (void*) 0x1 || 
		p == (void*) 0x2 || 
		p == (void*) 0x3 || 
		p == (void*) -1
		)
	{
		HMODULE module = LoadLibraryA("opengl32.dll");
		p = (void*)GetProcAddress(module, _procname);
	}

	return p;
}
