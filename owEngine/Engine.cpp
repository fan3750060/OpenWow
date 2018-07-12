#include "stdafx.h"

// General
#include "Engine.h"

// Additional
#include "GameStateManager.h"
#include "Perfomance.h"

CEngine::CEngine() : 
	m_IsNeedExit(false),
	framesCounter(0),
	m_FPS(0),
	framesTimer(0),
	_time(0),
	t(0)
{
	Log::Green("CEngine[]: Loading.");

	AddManager<IEngine>(this);
}

CEngine::~CEngine()
{
	OW_SAFEDELETE(_Render);
	OW_SAFEDELETE(_Perfomance);
	OW_SAFEDELETE(openGLConsole);

	Log::Green("CEngine[]: Destroy engine.");

	DelManager<IEngine>();
}

void CEngine::Init(IOpenGLAdapter* _OpenGLAdapter)
{
	_Render = new RenderGL();
	_Render->Init(_OpenGLAdapter);
	_Render->getAdapter()->MakeMainContext();

	_Perfomance = new Perfomance();


	openGLConsole = new CConsoleOpenGL();
	

	_OpenGLAdapter->CreateCursor();
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
	t = static_cast<uint32>(_Render->getAdapter()->GetTime() * 1000.0);
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
	_Bindings->m_UpdatableObjectCollection->Update(_Perfomance, _Render->getAdapter()->GetInput(), dTime, dDtTime);

	_Render->r.beginRendering();
	_Render->r.clear();

    //------------------------------------------------
    //-- Render3D
    //------------------------------------------------
	_Render->Set3D();
	_Bindings->m_Renderable3DObjectCollection->Render3D(_Perfomance);

    //------------------------------------------------
    //-- RenderUI
    //------------------------------------------------
	_Render->Set2D();
	_Bindings->m_RenderableUIObjectCollection->RenderUI();

	// Perfomance render
	_Perfomance->FrameEnd();
	_Perfomance->Render(vec2(5, 100));
	openGLConsole->RenderUI();

	//

	// Swap buffers
	if (!_Render->getAdapter()->SwapWindowBuffers() || m_IsNeedExit)
	{
		Log::Green("CEngine[]: Need exit.");
		return false;
	}

	// Caclulate FPS
	double currentTime = _Render->getAdapter()->GetTime();
	double delta = currentTime - framesTimer;
	framesCounter++;
	if (delta > 1.0)
	{
		m_FPS = static_cast<int>(static_cast<double>(framesCounter) / delta);
		framesTimer = currentTime;
		framesCounter = 0;

		_Render->getAdapter()->SetWindowTitle("FPS: " + to_string(m_FPS));
	}

	return true;
}
