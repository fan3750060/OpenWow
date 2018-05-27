#pragma once

#include "Engine.h"
class GameStateManager;
class UIWindow;

class GameState : public IUpdatable, public RenderableUIObject, public IInputListener
{
	friend GameStateManager;
public:
    GameState(Engine* _Engine);

	virtual bool Init();
	virtual void Destroy();

    virtual bool Set();
    virtual void Unset();

    virtual void Render(double t, double dt) {};

	bool IsInited() const { return m_IsInited; }
    bool IsCurrent() const { return m_IsCurrent; }

protected:
	bool      m_IsInited;
    bool      m_IsCurrent;
    UIWindow* m_Window;
	Engine*   m_Engine;
};
