#pragma once

class CGameState_World : public CGameState
{
	typedef CGameState base;
public:
	CGameState_World();
	virtual ~CGameState_World();

	// IGameState
	bool Init();
	void Destroy();

	virtual void OnPreRender(Render3DEventArgs& e);
	virtual void OnRender(Render3DEventArgs& e);
	virtual void OnPostRender(Render3DEventArgs& e);
	virtual void OnRenderUI(RenderUIEventArgs& e);

private:
	Viewport                             m_Viewport;

	RenderTechnique                      m_ForwardTechnique;
	RenderUITechnique                    m_UITechnique;

	std::shared_ptr<CCameraController>   m_CameraController;

	std::shared_ptr<Query>               m_FrameQuery;
	double                               m_FrameTime;
	std::shared_ptr<Scene3D>             m_3DScene;
	std::shared_ptr<SceneUI>             m_UIScene;


	std::shared_ptr<MapController>       m_MapController;

private:
	const vec4 g_ClearColor = vec4(0.39f, 0.58f, 0.93f, 1.0f);
};