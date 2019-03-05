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


	// Input events
	virtual void OnKeyPressed(KeyEventArgs& e);
	virtual void OnKeyReleased(KeyEventArgs& e);
	virtual void OnMouseButtonPressed(MouseButtonEventArgs& e);
	virtual void OnMouseButtonReleased(MouseButtonEventArgs& e);
	virtual void OnMouseMoved(MouseMotionEventArgs& e);
	virtual void OnMouseWheel(MouseWheelEventArgs& e);

	// Window events
	virtual void OnResize(ResizeEventArgs& e);

	// Update event
	virtual void OnUpdate(UpdateEventArgs& e);

	// Render events
	virtual void OnPreRender(Render3DEventArgs& e) = 0;
	virtual void OnRender(Render3DEventArgs& e) = 0;
	virtual void OnPostRender(Render3DEventArgs& e) = 0;
	virtual void OnRenderUI(RenderUIEventArgs& e) = 0;

protected:
	void SetCameraController(std::shared_ptr<ICameraController> CameraController);
	void UnsetCameraController();

protected:
	const IApplication*         m_Application;

	Viewport                    m_Viewport;
	std::shared_ptr<ICameraController> m_DefaultCameraController;

	const CGroupQuality&        m_QualitySettings;
	const CGroupVideo&          m_VideoSettings;

private: // Input events connections
	boost::signals2::connection OnKeyPressedConnection;
	boost::signals2::connection OnKeyReleasedConnection;
	boost::signals2::connection OnMouseButtonPressedConnection;
	boost::signals2::connection OnMouseButtonReleasedConnection;
	boost::signals2::connection OnMouseMovedConnection;
	boost::signals2::connection OnMouseWheelConnection;

private: // Windows events connection
	boost::signals2::connection OnResizeConnection;

private: // Update event connection
	boost::signals2::connection OnUpdateConnection;

private: // Render events connection
	boost::signals2::connection OnPreRenderConnection;
	boost::signals2::connection OnRenderConnection;
	boost::signals2::connection OnPostRenderConnection;
	boost::signals2::connection OnRenderUIConnection;

private:
	bool                        m_IsInited;
	bool                        m_IsCurrent;
};
