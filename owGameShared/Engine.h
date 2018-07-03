#pragma once

class CEngine : public IEngine
{
public:
	CEngine();
	~CEngine();

	void Init(IOpenGLAdapter* _OpenGLAdapter);
	void SetArguments(int argumentCount, char* arguments[]);

	// IEngine
	bool Tick();
	uint8 GetArgumentsCount() const override { return static_cast<uint32>(m_Arguments.size()); };
	string GetArgument(uint8 index) const override { return m_Arguments[index]; }
	uint32 GetFPS() const override { return m_FPS; }
	void SetNeedExit() override { m_IsNeedExit = true; }
	bool IsNeedExit() const override { return m_IsNeedExit;	}

private:
	vector<string>	m_Arguments;
	bool			m_IsNeedExit;


	uint32			framesCounter, m_FPS;
	double			framesTimer;

	uint32 t;
	uint32 last_t;
	uint32 _time;

	CConsoleOpenGL* openGLConsole;
};
