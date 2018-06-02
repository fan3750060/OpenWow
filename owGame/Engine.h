#pragma once

class CEngine : public IEngine
{
public:
	CEngine(IOpenGLAdapter* _OpenGLAdapter);
	~CEngine();

	void SetArguments(int argumentCount, char* arguments[]);

	// IEngine
	bool Tick();
	IOpenGLAdapter* GetAdapter() { return m_OpenGLAdapter; }
	string GetArgument(uint8 index) const { return m_Arguments[index]; }
	uint32 GetFPS() const { return framesPerSecond; }
	void SetNeedExit() override { m_IsNeedExit = true; }
	bool IsNeedExit() const override { return m_IsNeedExit;	}

private:
	vector<string>	m_Arguments;
	bool			m_IsNeedExit;


	uint32 framesCounter, framesPerSecond;
	double framesTimer;

	uint32 t;
	uint32 last_t;
	uint32 _time;

	IOpenGLAdapter* m_OpenGLAdapter;
};
