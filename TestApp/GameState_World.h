#pragma once


class CGameState_World : public CGameState
{
	typedef CGameState base;
public:
	CGameState_World();
	virtual ~CGameState_World();

	bool Init();
	void Destroy();

	virtual void OnPreRender(RenderEventArgs& e);
	virtual void OnRender(RenderEventArgs& e);
	virtual void OnPostRender(RenderEventArgs& e);
	virtual void OnRenderUI(RenderUIEventArgs& e);

private:
	Viewport g_Viewport;

	RenderTechnique g_ForwardTechnique;
	RenderUITechnique g_UITechnique;

	std::shared_ptr<CCameraController> g_CameraController;

	std::shared_ptr<Query> g_pFrameQuery;
	double g_FrameTime;
	std::shared_ptr<Scene> g_pScene;
	std::shared_ptr<UIScene> g_pUIScene;


	std::shared_ptr<MapController> m_MapController;

private:
	const vec4 g_ClearColor = vec4(0.39f, 0.58f, 0.93f, 1.0f);
};