#pragma once

class GameStateManager;

class CGameState : public IGameState, public IUpdatable, public IRenderableUI, public IInputListener
{
	friend GameStateManager;
public:
    CGameState();
	virtual ~CGameState();

	// IGameState
	virtual bool Init() override;
	virtual void Destroy() override;
    virtual bool Set() override;
    virtual void Unset() override;
	void SetInited(bool _value) override { m_IsInited = _value; }
	bool IsInited() const override { return m_IsInited; }
	void SetCurrent(bool _value) override { m_IsCurrent = _value; }
    bool IsCurrent() const override { return m_IsCurrent; }

protected:
    //UIWindow*			m_Window;
	//IEngine*			m_Engine;
	//IUIMgr*             m_UIMgr;

	CGroupQuality&		m_QualitySettings;
	CGroupVideo&		m_VideoSettings;

private:
	bool				m_IsInited;
	bool				m_IsCurrent;
};
