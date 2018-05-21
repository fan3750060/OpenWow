#pragma once

#include "InputListenerCollection.h"
#include "Renderable3DObjectCollection.h"
#include "RenderableUIObjectCollection.h"
#include "UpdatableObjectCollection.h"

class BindingController
{
	CLASS_INSTANCE(BindingController);
public:
	BindingController();
	~BindingController();

	void RegisterInputListener(InputListenerObject* _object);
	void RegisterRenderable3DObject(Renderable3DObject* _object, uint32 m_DrawOrder = 0);
	void RegisterRenderableUIObject(RenderableUIObject* _object, uint32 m_DrawOrder = 0);
	void RegisterUpdatableObject(UpdatableObject* _object);

	void UnregisterInputListener(InputListenerObject* _object);
	void UnregisterRenderable3DObject(Renderable3DObject* _object);
	void UnregisterRenderableUIObject(RenderableUIObject* _object);
	void UnregisterUpdatableObject(UpdatableObject* _object);

public:
	InputListenerObjectCollection* m_InputListenerObjectCollection;
	Renderable3DObjectCollection*  m_Renderable3DObjectCollection;
	RenderableUIObjectCollection*  m_RenderableUIObjectCollection;
	UpdatableObjectCollection*     m_UpdatableObjectCollection;
};

#define _Bindings BindingController::instance()