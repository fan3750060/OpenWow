#include "stdafx.h"

// General
#include "Engine.h"

// Additional
#include <ctime>
#include "GameStateManager.h"

Engine::Engine(IOpenGLAdapter* _OpenGLAdapter, int argumentCount, char* arguments[])
	: m_OpenGLAdapter(_OpenGLAdapter)
{
	// Arguments
	for (int i = 0; i < argumentCount; i++)
	{
		m_Arguments.push_back(arguments[i]);
	}

	// System settings
	Random::SetSeed(static_cast<unsigned long>(time(0)));

	// Files
	Log::Init();
	MPQArchiveStorage::InitCommonArchives();

	Log::Green("Engine[]: Loading.");

	// Init adapter
	assert1(m_OpenGLAdapter);
	assert1(m_OpenGLAdapter->Init());

	// Load modules
	_Render->Init();

	needExit = false;

	framesCounter = 0;
	framesPerSecond = 0;
	framesTimer = 0;

	t = 0;

}

Engine::~Engine()
{
	Log::Green("Engine[]: Destroy engine.");

	MPQArchiveStorage::ClearArchives();

	exit(0);
}

bool Engine::Tick()
{
	last_t = t;
	t = static_cast<uint32>(m_OpenGLAdapter->GetTime() * 1000.0);
	uint32 dt = t - last_t;
	_time += dt;

	double dTime = static_cast<double>(_time) / 1000.0;
	double dDtTime = static_cast<double>(dt) / 1000.0;

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
	_Bindings->m_Renderable3DObjectCollection->Render3D(dTime, dDtTime);

    //------------------------------------------------
    //-- RenderUI
    //------------------------------------------------
	_Render->Set2D();
	_Bindings->m_RenderableUIObjectCollection->RenderUI();

	//

	// Swap buffers
	if (!m_OpenGLAdapter->SwapWindowBuffers())
	{
		if (!needExit)
		{
			Log::Green("Engine[]: Need exit.");
			needExit = true;
			return false;
		}
	}

	// Caclulate FPS
	double currentTime = m_OpenGLAdapter->GetTime();
	double delta = currentTime - framesTimer;
	framesCounter++;
	if (delta > 1.0)
	{
		framesPerSecond = static_cast<int>(static_cast<double>(framesCounter) / delta);
		framesTimer = currentTime;
		framesCounter = 0;

		m_OpenGLAdapter->SetWindowTitle("FPS: " + to_string(framesPerSecond));
	}

	return true;
}
