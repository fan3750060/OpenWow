#include "stdafx.h"

// General
#include "Engine.h"

// Additional
#include "GameStateManager.h"

CEngine::CEngine(IOpenGLAdapter* _OpenGLAdapter) : 
	m_OpenGLAdapter(_OpenGLAdapter),
	m_IsNeedExit(false),
	framesCounter(0),
	m_FPS(0),
	framesTimer(0),
	_time(0),
	t(0)
{
	Log::Green("CEngine[]: Loading.");



	// Load modules
	m_OpenGLAdapter->MakeContextMainThread();
	_Render->Init(m_OpenGLAdapter, m_OpenGLAdapter->GetMainCont());

	AddManager<IEngine>(this);
}

CEngine::~CEngine()
{
	Log::Green("CEngine[]: Destroy engine.");

	DelManager<IEngine>();
}

void CEngine::SetArguments(int argumentCount, char* arguments[])
{
	for (int i = 0; i < argumentCount; i++)
	{
		m_Arguments.push_back(arguments[i]);
	}
}

bool CEngine::Tick()
{
	last_t = t;
	t = static_cast<uint32>(m_OpenGLAdapter->GetTime() * 1000.0);
	uint32 dt = t - last_t;
	_time += dt;

	double dTime = static_cast<double>(_time);
	double dDtTime = static_cast<double>(dt);

	_Perfomance->FrameBegin();

	//GetManager<ILoader>()->LoadAll();
	//GetManager<ILoader>()->DeleteAll();

	IMapManager* mapMgr = nullptr;
	if ((mapMgr = GetManager<IMapManager>()) != nullptr)
	{
		mapMgr->Update();
	}

    //------------------------------------------------
	//-- Update
    //------------------------------------------------
	_Bindings->m_UpdatableObjectCollection->Update(dTime, dDtTime);

	_Render->r.beginRendering();
	_Render->r.clear();

    //------------------------------------------------
    //-- Render3D
    //------------------------------------------------
	_Render->Set3D();
	_Bindings->m_Renderable3DObjectCollection->Render3D();

    //------------------------------------------------
    //-- RenderUI
    //------------------------------------------------
	_Render->Set2D();
	_Bindings->m_RenderableUIObjectCollection->RenderUI();

	//

	// Swap buffers
	if (!m_OpenGLAdapter->SwapWindowBuffers() || m_IsNeedExit)
	{
		Log::Green("CEngine[]: Need exit.");
		return false;
	}

	// Caclulate FPS
	double currentTime = m_OpenGLAdapter->GetTime();
	double delta = currentTime - framesTimer;
	framesCounter++;
	if (delta > 1.0)
	{
		m_FPS = static_cast<int>(static_cast<double>(framesCounter) / delta);
		framesTimer = currentTime;
		framesCounter = 0;

		m_OpenGLAdapter->SetWindowTitle("FPS: " + to_string(m_FPS));
	}

	return true;
}
