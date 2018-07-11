#pragma once

#include "RenderTypes.h"
#include "RenderStates.h"
#include "Technique.h"

struct RenderDrawCall
{
	//typedef void(*BindUniformsFunc)(Technique* _techique);

	RenderDrawCall() :
		renderState(nullptr),
		technique(nullptr),
		bindUniformsFunc(nullptr),
		firstIndex(0),
		numIndices(0),
		firstVert(0),
		numVerts(0)
	{}

	RenderState* renderState;
	Technique* technique;
	void(*bindUniformsFunc)(Technique* _techique);
	uint32 firstIndex;
	uint32 numIndices;
	uint32 firstVert;
	uint32 numVerts;
};

class RenderQueue
{
public:
	RenderQueue(RenderDevice* _RenderDevice);

	void PushCall(RenderDrawCall& _call);
	void ProcessDrawCalls();

private:
	vector<RenderDrawCall> m_DrawCalls;

	RenderDevice* m_RD;
};