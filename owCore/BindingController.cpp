#include "stdafx.h"

// General
#include "BindingController.h"

CBindingController::CBindingController()
{
	m_InputListenerObjectCollection = new CInputListenerObjectCollection();
	m_Renderable3DObjectCollection = new CRenderable3DObjectCollection();
	m_RenderableUIObjectCollection = new CRenderableUIObjectCollection();
	m_UpdatableObjectCollection = new CUpdatableObjectCollection();
}

CBindingController::~CBindingController()
{
	OutputDebugString(L"CBindingController destroyed.\n");
}

void CBindingController::RegisterInputListener(IInputListener* _object)
{
	m_InputListenerObjectCollection->RegisterObject(_object);
}

void CBindingController::RegisterRenderable3DObject(CRenderable3DObject* _object, uint32 _DrawOrder)
{
	m_Renderable3DObjectCollection->RegisterObject(_object, _DrawOrder);
}

void CBindingController::RegisterRenderableUIObject(CRenderableUIObject* _object, uint32 _DrawOrder)
{
	m_RenderableUIObjectCollection->RegisterObject(_object, _DrawOrder);
}

void CBindingController::RegisterUpdatableObject(IUpdatable* _object)
{
	m_UpdatableObjectCollection->RegisterObject(_object);
}

void CBindingController::UnregisterInputListener(IInputListener * _object)
{
	m_InputListenerObjectCollection->UnregisterObject(_object);
}

void CBindingController::UnregisterRenderable3DObject(CRenderable3DObject* _object)
{
	m_Renderable3DObjectCollection->UnregisterObject(_object);
}

void CBindingController::UnregisterRenderableUIObject(CRenderableUIObject * _object)
{
	m_RenderableUIObjectCollection->UnregisterObject(_object);
}

void CBindingController::UnregisterUpdatableObject(IUpdatable * _object)
{
	m_UpdatableObjectCollection->UnregisterObject(_object);
}
