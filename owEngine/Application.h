#pragma once

#define IS_DX11 1

#include "GameState.h"

class Application : public Object, public IGameStateManager
{
public:
	Application();
	virtual ~Application();

	static Application& Get();

	// IApplication
	bool                            Load();
	int                             Run();
	void                            Stop();

	std::shared_ptr<RenderWindow>   CreateRenderWindow(cstring title, int windowWidth, int windowHeight, bool vSync = false);

	std::shared_ptr<IRenderDevice>  GetRenderDevice();
	std::shared_ptr<RenderWindow>   GetRenderWindow();
	HINSTANCE                       Get_HINSTANCE() const;
	HWND                            Get_HWND() const;

	// IGameStateManager
	void AddGameState(GameStatesNames::List _name, std::shared_ptr<IGameState> _gameState) override;
	bool SetGameState(GameStatesNames::List _name) override;
	bool SetGameState(std::shared_ptr<IGameState> _newGameState) override;
	std::shared_ptr<IGameState> GetGameState() override;

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
	void OnRender(Render3DEventArgs& e);
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

	// IGameStateManager
	std::shared_ptr<IGameState>                                     m_CurrentGameState;
	std::map<GameStatesNames::List, std::shared_ptr<IGameState>>    m_GameStatesCollection;

private:
	const char* c_RenderWindow_ClassName = "RenderWindowClass";
};

#define _RenderDevice Application::Get().GetRenderDevice()
