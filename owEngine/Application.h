#pragma once

#define IS_DX11 1

class Application : public Object
{
public:
	Application();
	virtual ~Application();

	static Application& Get();

	bool Load();
	int Run();
	void Stop();

	std::shared_ptr<RenderWindow> CreateRenderWindow(cstring title, int windowWidth, int windowHeight, bool vSync = false);

	std::shared_ptr<IRenderDevice> GetRenderDevice();
	std::shared_ptr<RenderWindow> GetRenderWindow();

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
	HWND            m_hWindow;

	std::shared_ptr<IRenderDevice> m_pRenderDevice;
	std::shared_ptr<RenderWindow>  m_pWindow;

private:
	const char* c_RenderWindow_ClassName = "RenderWindowClass";
};

#define _RenderDevice Application::Get().GetRenderDevice()
