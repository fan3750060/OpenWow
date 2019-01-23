#pragma once

class Camera;
class PipelineState;
struct Viewport;

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
	RenderEventArgs(const Object& caller, float fDeltaTime, float fTotalTime, uint64_t frameCounter, const Camera* camera = nullptr, const PipelineState* pipelineState = nullptr, const Object* node = nullptr)
		: base(caller)
		, ElapsedTime(fDeltaTime)
		, TotalTime(fTotalTime)
		, FrameCounter(frameCounter)
		, Camera(camera)
		, PipelineState(pipelineState)
		, Node(node)
	{}

	float ElapsedTime;
	float TotalTime;
	int64_t FrameCounter;

	const Camera* Camera;
	const PipelineState* PipelineState;
	const Object* Node;
};
typedef Delegate<RenderEventArgs> RenderEvent;


class RenderUIEventArgs : public EventArgs
{
public:
	typedef EventArgs base;
	RenderUIEventArgs(const Object& caller, float fDeltaTime, float fTotalTime, uint64_t frameCounter, const Viewport* viewport = nullptr)
		: base(caller)
		, ElapsedTime(fDeltaTime)
		, TotalTime(fTotalTime)
		, FrameCounter(frameCounter)
		, Viewport(viewport)
	{}

	float ElapsedTime;
	float TotalTime;
	int64_t FrameCounter;

	const Viewport* Viewport;
};
typedef Delegate<RenderUIEventArgs> RenderUIEvent;



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