#include "stdafx.h"

// General
#include "BindingController.h"

BindingController::BindingController()
{
	m_InputListenerObjectCollection = new InputListenerObjectCollection();
	m_Renderable3DObjectCollection = new Renderable3DObjectCollection();
	m_RenderableUIObjectCollection = new RenderableUIObjectCollection();
	m_UpdatableObjectCollection = new UpdatableObjectCollection();
}

BindingController::~BindingController()
{
}

void BindingController::RegisterInputListener(InputListenerObject* _object)
{
	m_InputListenerObjectCollection->RegisterObject(_object);
}

void BindingController::RegisterRenderable3DObject(Renderable3DObject* _object, uint32 _DrawOrder)
{
	m_Renderable3DObjectCollection->RegisterObject(_object, _DrawOrder);
}

void BindingController::RegisterRenderableUIObject(RenderableUIObject* _object, uint32 _DrawOrder)
{
	m_RenderableUIObjectCollection->RegisterObject(_object, _DrawOrder);
}

void BindingController::RegisterUpdatableObject(UpdatableObject* _object)
{
	m_UpdatableObjectCollection->RegisterObject(_object);
}

void BindingController::UnregisterInputListener(InputListenerObject * _object)
{
	m_InputListenerObjectCollection->UnregisterObject(_object);
}

void BindingController::UnregisterRenderable3DObject(Renderable3DObject * _object)
{
	m_Renderable3DObjectCollection->UnregisterObject(_object);
}

void BindingController::UnregisterRenderableUIObject(RenderableUIObject * _object)
{
	m_RenderableUIObjectCollection->UnregisterObject(_object);
}

void BindingController::UnregisterUpdatableObject(UpdatableObject * _object)
{
	m_UpdatableObjectCollection->UnregisterObject(_object);
}
