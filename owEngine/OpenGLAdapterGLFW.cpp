#include "stdafx.h"

// Include
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// General
#include "OpenGLAdapterGLFW.h"

// Error callback
void GLFWErrorCallback(int error, const char* description)
{
	Log::Error("GLFW[]: Error [%d] (%s).", error, description);
}

// Resize callback
void GLFWFramebufferCallback(GLFWwindow* _window, int _width, int _height)
{
	//_Engine->GetAdapter()->SetWindowSize(_width, _height);
}

CInput* input;

// Input callbacks
void GLFWMousePositionCallback(GLFWwindow* window, double xpos, double ypos) { input->MousePositionCallback(vec2(static_cast<int>(xpos), static_cast<int>(ypos))); }
void GLFWMouseCallback(GLFWwindow* window, int32 button, int32 action, int32 mods) { input->MouseCallback(button, action, mods); }
void GLFWMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) { input->MouseScrollCallback(static_cast<int>(yoffset)); }
void GLFWKeyboardCallback(GLFWwindow* window, int32 key, int32 scancode, int32 action, int32 mods) { input->KeyboardCallback(key, scancode, action, mods); }
void GLFWCharCallback(GLFWwindow* window, uint32 _char) { input->CharCallback(_char); }
void GLFWDropCallback(GLFWwindow*, int num, const char** fnames) { input->DropFilesCallback(num, fnames); }

//---------------------------------------------------------

OpenGLAdapter_GLFW::OpenGLAdapter_GLFW()
{
	CGroupVideo& groupVideo = GetSettingsGroup<CGroupVideo>();
	CGroupOpenGL& openglSettings = GetSettingsGroup<CGroupOpenGL>();

	glfwSetErrorCallback(GLFWErrorCallback);

	if (!glfwInit())
	{
		Log::Error("GLFW[]: Error while init GLFW!");
		fail1();
	}
	Log::Print("GLFW[]: Version [%s].", glfwGetVersionString());

	// Set window options

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openglSettings.majorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openglSettings.minorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);
	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	// Monitor

	primaryMonitor = glfwGetPrimaryMonitor();
	const char* primaryMonitorName = glfwGetMonitorName(primaryMonitor);
	Log::Print("GLFW[]: Primary monitor name [%s]", primaryMonitorName);

	// Videomode contain settings

	const GLFWvidmode* primaryMonitorMode = glfwGetVideoMode(primaryMonitor);
	Log::Print("GLFW[]: Primary monitor resolution [%d, %d]", primaryMonitorMode->width, primaryMonitorMode->height);

	// Physical size in mm
	int32 primaryMonitorPhysicalWidth, primaryMonitorPhysicalHeight;
	glfwGetMonitorPhysicalSize(primaryMonitor, &primaryMonitorPhysicalWidth, &primaryMonitorPhysicalHeight);
	Log::Print("GLFW[]: Primary monitor physical size [%d, %d]", primaryMonitorPhysicalWidth, primaryMonitorPhysicalHeight);

	
	// Create GLFW Thread window
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	m_ThreadWindow = glfwCreateWindow(1, 1, "Thread Window", NULL, NULL);

	// Create GLFW window
	glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
	m_Window = glfwCreateWindow(groupVideo.windowSizeX, groupVideo.windowSizeY, "Default m_Window title.", nullptr, m_ThreadWindow);

	// Move window to center
	uint32 windowPositionX = (primaryMonitorMode->width / 2) - (groupVideo.windowSizeX / 2);
	uint32 windowPositionY = (primaryMonitorMode->height / 2) - (groupVideo.windowSizeY / 2);
	glfwSetWindowPos(m_Window, windowPositionX, windowPositionY);
	Log::Print("GLFW[]: Window position [%d, %d]", windowPositionX, windowPositionY);

	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(0);

	// Time
	glfwSetTime(0.0);

	// Resize callback
	glfwSetFramebufferSizeCallback(m_Window, GLFWFramebufferCallback);

	// Input
	m_Input = new CInput();
	input = m_Input;
	glfwSetInputMode(m_Window, GLFW_STICKY_KEYS, 0);
	glfwSetMouseButtonCallback(m_Window, GLFWMouseCallback);
	glfwSetCursorPosCallback(m_Window, GLFWMousePositionCallback);
	glfwSetScrollCallback(m_Window, GLFWMouseScrollCallback);
	glfwSetKeyCallback(m_Window, GLFWKeyboardCallback);
	glfwSetCharCallback(m_Window, GLFWCharCallback);
	glfwSetDropCallback(m_Window, GLFWDropCallback);
}

OpenGLAdapter_GLFW::~OpenGLAdapter_GLFW()
{
	delete m_Input;

	glfwTerminate();
}

bool OpenGLAdapter_GLFW::SwapWindowBuffers()
{
	glfwSwapBuffers(m_Window);
	glfwPollEvents();

	return !glfwWindowShouldClose(m_Window);
}

double OpenGLAdapter_GLFW::GetTime()
{
	return glfwGetTime();
}

CInput* OpenGLAdapter_GLFW::GetInput()
{
	return m_Input;
}

void OpenGLAdapter_GLFW::MakeMainContext()
{
	glfwMakeContextCurrent(m_Window);
}

void OpenGLAdapter_GLFW::MakeThreadContext()
{
	glfwMakeContextCurrent(m_ThreadWindow);
}
//

void OpenGLAdapter_GLFW::SetWindowSize(int32 _width, int32 _height)
{
	glfwSetWindowSize(m_Window, _width, _height);
	//_Render->OnWindowResized(_width, _height);
}

void OpenGLAdapter_GLFW::SetWindowTitle(cstring _title)
{
	glfwSetWindowTitle(m_Window, _title.c_str());
}

void OpenGLAdapter_GLFW::CreateCursor()
{
	/*R_Texture* tex = _Render->TexturesMgr()->Add("Interface\\CURSOR\\Point.blp");

	GLFWimage* image = new GLFWimage;
	image->width = 32;
	image->height = 32;
	image->pixels = new uint8[image->width * image->height * 4];

	tex->getTextureData(0, 0, image->pixels);

	GLFWcursor* cur = glfwCreateCursor(image, 0, 0);
	glfwSetCursor(m_Window, cur);*/
}

void OpenGLAdapter_GLFW::ShowCursor()
{
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void OpenGLAdapter_GLFW::HideCursor()
{
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void OpenGLAdapter_GLFW::setMousePosition(cvec2 _mousePosition)
{
	glfwSetCursorPos(m_Window, static_cast<double>(_mousePosition.x), static_cast<double>(_mousePosition.y));
}

int OpenGLAdapter_GLFW::isExtensionSupported(const char* _extension)
{
	return glfwExtensionSupported(_extension);
}

void* OpenGLAdapter_GLFW::getProcAddress(const char* _procname)
{
	return (void*)glfwGetProcAddress(_procname);
}
