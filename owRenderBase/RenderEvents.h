#pragma once

class Camera;
class PipelineState;

class UpdateEventArgs : public EventArgs
{
public:
	typedef EventArgs base;
	UpdateEventArgs(const Object& caller, float fDeltaTime, float fTotalTime)
		: base(caller)
		, ElapsedTime(fDeltaTime)
		, TotalTime(fTotalTime)
	{}

	float ElapsedTime;
	float TotalTime;
};
typedef Delegate<UpdateEventArgs> UpdateEvent;



class RenderEventArgs : public EventArgs
{
public:
	typedef EventArgs base;
	RenderEventArgs(const Object& caller, float fDeltaTime, float fTotalTime, uint64_t frameCounter, Camera* camera = nullptr, PipelineState* pipelineState = nullptr)
		: base(caller)
		, ElapsedTime(fDeltaTime)
		, TotalTime(fTotalTime)
		, FrameCounter(frameCounter)
		, Camera(camera)
		, PipelineState(pipelineState)
	{}

	float ElapsedTime;
	float TotalTime;
	int64_t FrameCounter;
	Camera* Camera;
	PipelineState* PipelineState;
};
typedef Delegate<RenderEventArgs> RenderEvent;



class UserEventArgs : public EventArgs
{
public:
	typedef EventArgs base;
	UserEventArgs(const Object& caller, int code, void* data1, void* data2)
		: base(caller)
		, Code(code)
		, Data1(data1)
		, Data2(data2)
	{}

	int     Code;
	void*   Data1;
	void*   Data2;
};
typedef Delegate<UserEventArgs> UserEvent;