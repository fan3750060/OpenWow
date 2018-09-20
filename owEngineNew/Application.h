#pragma once

class Application : public Object
{
public:
	Application();
	virtual ~Application();

	// Get the singleton instance to the application.
	static Application& Get();

	// Run the application
	virtual int Run();

	// Stop the app from running and close the application window.
	virtual void Stop();

	// Create a new render Window with the specified dimensions.
	RenderWindow* CreateRenderWindow(cstring title, int windowWidth, int windowHeight, bool vSync = false);

	// Get the render device associated with this application.
	RenderDevice* GetRenderDevice();

	// Get the module handle for this instance of the application.
	// TODO: This should be abstracted away into a specific Application class.
	HINSTANCE GetModuleHandle() const;

	// Application execution events
	Event           Initialize;
	UpdateEvent     Update;

	// Called when the application is exiting.
	Event           Exit;

	// This event is called when the application is going to be terminated.
	Event			Terminate;
	// This event is called after the application has been terminated.
	Event           Terminated;

	// User events
	UserEvent        UserEvent;

protected:

	// Invoked when the application is initialized.
	// This is called when the application is run.
	// This allows windows that have been created to receive this event.
	virtual void OnInitialize(EventArgs& e);

	virtual void OnUpdate(UpdateEventArgs& e);

	virtual void OnRender(RenderEventArgs& e);

	// The application instance is going to be terminated.
	virtual void OnTerminate(EventArgs& e);

	// The application instance has been terminated.
	virtual void OnTerminated(EventArgs& e);

	// The user requested to exit the application
	virtual void OnExit(EventArgs& e);

	// A user event was generated.
	virtual void OnUserEvent(UserEventArgs& e);

private:
	bool            m_bIsInitialized;
	bool            m_bIsRunning;

	// Handle to the module.
	HINSTANCE       m_hInstance;
	RenderDevice*   m_pRenderDevice;
	RenderWindow*   m_Windows;
};