#pragma once

//#define IS_DX11 1

class Application : public Object
{
public:
	Application();
	virtual ~Application();

	// Get the singleton instance to the application.
	static Application& Get();

	int Run();
	void Stop();

	RenderWindow* CreateRenderWindow(cstring title, int windowWidth, int windowHeight, bool vSync = false);

	IRenderDevice* GetRenderDevice();
	RenderWindow* GetRenderWindow();

	HINSTANCE GetModuleHandle() const;

	// Application execution events
	Event           Initialize;
	UpdateEvent     Update;
	Event			Terminate;
	Event           Terminated;
	Event           Exit;
	UserEvent       UserEvent;

protected:
	void OnInitialize(EventArgs& e);
	void OnUpdate(UpdateEventArgs& e);
	void OnRender(RenderEventArgs& e);
	void OnRenderUI(RenderUIEventArgs& e);
	void OnTerminate(EventArgs& e);
	void OnTerminated(EventArgs& e);
	void OnExit(EventArgs& e);
	void OnUserEvent(UserEventArgs& e);

private:
	bool            m_bIsInitialized;
	bool            m_bIsRunning;

	// Handle to the module.
	HINSTANCE       m_hInstance;
	IRenderDevice*   m_pRenderDevice;
	RenderWindow*   m_Windows;
};

#define _RenderDevice Application::Get().GetRenderDevice()
