#pragma once

#include "InputListenerCollection.h"
#include "Renderable3DObjectCollection.h"
#include "RenderableUIObjectCollection.h"
#include "UpdatableObjectCollection.h"

class CBindingController
{
public:
	CBindingController();
	~CBindingController();

	void RegisterInputListener(IInputListener* _object, uint8 _priority = 0);
	void RegisterRenderable3DObject(IRenderable3D* _object, uint32 m_DrawOrder = 0);
	void RegisterRenderableUIObject(IRenderableUI* _object, uint32 m_DrawOrder = 0);
	void RegisterUpdatableObject(IUpdatable* _object);

	void UnregisterInputListener(IInputListener* _object);
	void UnregisterRenderable3DObject(IRenderable3D* _object);
	void UnregisterRenderableUIObject(IRenderableUI* _object);
	void UnregisterUpdatableObject(IUpdatable* _object);

public:
	CInputListenerObjectCollection* m_InputListenerObjectCollection;
	CRenderable3DObjectCollection*  m_Renderable3DObjectCollection;
	CRenderableUIObjectCollection*  m_RenderableUIObjectCollection;
	CUpdatableObjectCollection*     m_UpdatableObjectCollection;
};

extern CBindingController* _Bindings;