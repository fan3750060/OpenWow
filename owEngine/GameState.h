#pragma once

class CGameStateManager;

class CGameState : public IGameState
{
	friend CGameStateManager;
public:
    CGameState(const IApplication * _application);
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

	//

	virtual void OnResize(ResizeEventArgs& e);

	virtual void OnPreRender(Render3DEventArgs& e) = 0;
	virtual void OnRender(Render3DEventArgs& e) = 0;
	virtual void OnPostRender(Render3DEventArgs& e) = 0;
	virtual void OnRenderUI(RenderUIEventArgs& e) = 0;

protected:
	Viewport                    m_Viewport;

	const IApplication*         m_Application;

	const CGroupQuality&        m_QualitySettings;
	const CGroupVideo&          m_VideoSettings;

private:
	bool                        m_IsInited;
	bool                        m_IsCurrent;
};
