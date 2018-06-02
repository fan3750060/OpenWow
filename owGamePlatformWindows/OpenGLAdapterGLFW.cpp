#include "stdafx.h"

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

Input* _input = nullptr;

// Input callbacks
void GLFWMousePositionCallback(GLFWwindow* window, double xpos, double ypos) { _input->MousePositionCallback(vec2(static_cast<int>(xpos), static_cast<int>(ypos))); }
void GLFWMouseCallback(GLFWwindow* window, int32 button, int32 action, int32 mods) { _input->MouseCallback(button, action, mods); }
void GLFWMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) { _input->MouseScrollCallback(static_cast<int>(yoffset)); }
void GLFWKeyboardCallback(GLFWwindow* window, int32 key, int32 scancode, int32 action, int32 mods) { _input->KeyboardCallback(key, scancode, action, mods); }
void GLFWCharCallback(GLFWwindow* window, uint32 _char) { _input->CharCallback(_char); }

//---------------------------------------------------------

OpenGLAdapter_GLFW::OpenGLAdapter_GLFW()
{
	CGroupVideo& groupVideo = GetSettingsGroup<CGroupVideo>();

	glfwSetErrorCallback(GLFWErrorCallback);

	if (!glfwInit())
	{
		Log::Error("GLFW[]: Error while init GLFW!");
		fail1();
	}
	Log::Print("GLFW[]: Version [%s].", glfwGetVersionString());

	// Set window options

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

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

	// Create GLFW window

	m_Window = glfwCreateWindow(groupVideo.windowSizeX, groupVideo.windowSizeY, "Default m_Window title.", nullptr, nullptr);

	// Move window to center
	uint32 windowPositionX = (primaryMonitorMode->width / 2) - (groupVideo.windowSizeX / 2);
	uint32 windowPositionY = (primaryMonitorMode->height / 2) - (groupVideo.windowSizeY / 2);
	glfwSetWindowPos(m_Window, windowPositionX, windowPositionY);
	Log::Print("GLFW[]: Window position [%d, %d]", windowPositionX, windowPositionY);

	//nativeWindow = glfwGetWin32Window(window);
	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(1);

	// Time
	glfwSetTime(0.0);

	// Resize callback
	glfwSetFramebufferSizeCallback(m_Window, GLFWFramebufferCallback);

	// Input
	m_Input = new Input();
	_input = m_Input;
	glfwSetInputMode(m_Window, GLFW_STICKY_KEYS, 0);
	glfwSetMouseButtonCallback(m_Window, GLFWMouseCallback);
	glfwSetCursorPosCallback(m_Window, GLFWMousePositionCallback);
	glfwSetScrollCallback(m_Window, GLFWMouseScrollCallback);
	glfwSetKeyCallback(m_Window, GLFWKeyboardCallback);
	glfwSetCharCallback(m_Window, GLFWCharCallback);
}

OpenGLAdapter_GLFW::~OpenGLAdapter_GLFW()
{
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

Input* OpenGLAdapter_GLFW::GetInput()
{
	return m_Input;
}

HGLRC OpenGLAdapter_GLFW::GetWGLContext()
{
	return glfwGetWGLContext(m_Window);
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

void OpenGLAdapter_GLFW::ShowCursor()
{
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void OpenGLAdapter_GLFW::HideCursor()
{
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void OpenGLAdapter_GLFW::SetMousePosition(cvec2 _mousePosition)
{
	glfwSetCursorPos(m_Window, static_cast<double>(_mousePosition.x), static_cast<double>(_mousePosition.y));
}