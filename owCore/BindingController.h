#pragma once

#include "InputListenerCollection.h"
#include "Renderable3DObjectCollection.h"
#include "RenderableUIObjectCollection.h"
#include "UpdatableObjectCollection.h"

class CBindingController
{
	CLASS_INSTANCE(CBindingController);
public:
	CBindingController();
	~CBindingController();

	void RegisterInputListener(IInputListener* _object);
	void RegisterRenderable3DObject(CRenderable3DObject* _object, uint32 m_DrawOrder = 0);
	void RegisterRenderableUIObject(CRenderableUIObject* _object, uint32 m_DrawOrder = 0);
	void RegisterUpdatableObject(IUpdatable* _object);

	void UnregisterInputListener(IInputListener* _object);
	void UnregisterRenderable3DObject(CRenderable3DObject* _object);
	void UnregisterRenderableUIObject(CRenderableUIObject* _object);
	void UnregisterUpdatableObject(IUpdatable* _object);

public:
	CInputListenerObjectCollection* m_InputListenerObjectCollection;
	CRenderable3DObjectCollection*  m_Renderable3DObjectCollection;
	CRenderableUIObjectCollection*  m_RenderableUIObjectCollection;
	CUpdatableObjectCollection*     m_UpdatableObjectCollection;
};

#define _Bindings CBindingController::instance()