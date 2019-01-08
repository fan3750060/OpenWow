#include "stdafx.h"

// General
#include "BindingController.h"

CBindingController* _Bindings;

CBindingController::CBindingController()
{
	m_InputListenerObjectCollection = new CInputListenerObjectCollection();
	m_RenderableUIObjectCollection = new CRenderableUIObjectCollection();
	m_UpdatableObjectCollection = new CUpdatableObjectCollection();
}

CBindingController::~CBindingController()
{
	delete m_InputListenerObjectCollection;
	delete m_RenderableUIObjectCollection;
	delete m_UpdatableObjectCollection;
	OutputDebugString("CBindingController destroyed.\n");
}

void CBindingController::RegisterInputListener(IInputListener* _object, uint8 _priority)
{
	m_InputListenerObjectCollection->RegisterObject(_object, _priority);
}

void CBindingController::RegisterRenderableUIObject(IRenderableUI* _object, uint32 _DrawOrder)
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

void CBindingController::UnregisterRenderableUIObject(IRenderableUI* _object)
{
	m_RenderableUIObjectCollection->UnregisterObject(_object);
}

void CBindingController::UnregisterUpdatableObject(IUpdatable * _object)
{
	m_UpdatableObjectCollection->UnregisterObject(_object);
}
