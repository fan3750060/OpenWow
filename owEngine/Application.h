#pragma once

#define IS_DX11 1

#include "GameState.h"
#include "Loader.h"

class Application : public Object, public IApplication, public IGameStateManager
{
public:
	Application();
	Application(HINSTANCE hInstance);
	virtual ~Application();

	static IApplication& Get();

	bool                            Load();
	int                             Run();
	void                            Stop();

	

	std::shared_ptr<RenderWindow>   CreateRenderWindow(cstring title, int windowWidth, int windowHeight, bool vSync = false);

	CLoader*						GetLoader();
	HINSTANCE                       Get_HINSTANCE() const;
	HWND                            Get_HWND() const;

	// IApplication
	int                             DoRun();
	std::shared_ptr<IRenderDevice>  GetRenderDevice() const override;
	void                            SetRenderDevice(std::shared_ptr<IRenderDevice> _renderDevice) override;
	std::shared_ptr<RenderWindow>   GetRenderWindow() const override;
	void                            SetRenderWindow(std::shared_ptr<RenderWindow> _renderWindow) override;

	// IGameStateManager
	void                            AddGameState(GameStatesNames::List _name, std::shared_ptr<IGameState> _gameState) override;
	bool                            SetGameState(GameStatesNames::List _name) override;
	bool                            SetGameState(std::shared_ptr<IGameState> _newGameState) override;
	std::shared_ptr<IGameState>     GetGameState() override;

	// Application execution events
	Event                           Initialize;
	UpdateEvent                     Update;
	Event			                Terminate;
	Event                           Terminated;
	Event                           Exit;
	UserEvent                       UserEvent;

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

protected:
	void                            OnInitialize(EventArgs& e);
	void                            OnUpdate(UpdateEventArgs& e);
	void                            OnRender(Render3DEventArgs& e);
	void                            OnRenderUI(RenderUIEventArgs& e);
	void                            OnTerminate(EventArgs& e);
	void                            OnTerminated(EventArgs& e);
	void                            OnExit(EventArgs& e);
	void                            OnUserEvent(UserEventArgs& e);

private:
	bool                            m_bIsInitialized;
	bool                            m_bIsRunning;

	// Handle to the module.
	CLoader         m_Loader;
	HINSTANCE       m_HINSTANCE;
	HWND            m_HWND;

	std::shared_ptr<IRenderDevice> m_pRenderDevice;
	std::shared_ptr<RenderWindow>  m_pWindow;

	// IGameStateManager
	std::shared_ptr<IGameState>                                     m_CurrentGameState;
	std::map<GameStatesNames::List, std::shared_ptr<IGameState>>    m_GameStatesCollection;

private:
	const char* c_RenderWindow_ClassName = "RenderWindowClass";
};

extern IApplication* _ApplicationInstance;

#define _RenderDevice Application::Get().GetRenderDevice()
