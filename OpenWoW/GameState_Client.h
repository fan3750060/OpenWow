#pragma once

class CGameState_Client : public CGameState
{
	typedef CGameState base;
public:
	CGameState_Client(const IApplication * Application);
	virtual ~CGameState_Client();

	void S_CharEnum(CByteBuffer& _buff);

	// IGameState
	bool Init();
	void Destroy();

	virtual void OnPreRender(Render3DEventArgs& e);
	virtual void OnRender(Render3DEventArgs& e);
	virtual void OnPostRender(Render3DEventArgs& e);
	virtual void OnRenderUI(RenderUIEventArgs& e);


private:
	void Load3D();
	void LoadUI();

private:
	Viewport                             m_Viewport;

	std::shared_ptr<CFreeCameraController>   m_CameraController;

	std::shared_ptr<Query>               m_FrameQuery;
	double                               m_FrameTime;
	std::shared_ptr<Scene3D>             m_3DScene;
	std::shared_ptr<CUIScene>             m_UIScene;


	std::shared_ptr<MapController>       m_MapController;
	std::shared_ptr<CWoWClient>m_authWorldController;

	std::shared_ptr<CUITextNode>              m_CameraPosText;
	std::shared_ptr<CUITextNode>              m_CameraRotText;

private:
	const vec4 g_ClearColor = vec4(0.39f, 0.58f, 0.93f, 1.0f);
};