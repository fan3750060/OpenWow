#include "stdafx.h"

// General
#include "UIMgr.h"

UIMgr::UIMgr()
{
    m_IDCounter = 0;
    m_RootElement = nullptr;
    m_FocusedElement = nullptr;

    //

	_Bindings->RegisterUpdatableObject(this);
	_Bindings->RegisterRenderableUIObject(this, 500);
	_Bindings->RegisterInputListener(this);

	m_FontsMgr = new FontsManager(&_Render->r);

	AddManager<IUIMgr>(this);
}

UIMgr::~UIMgr()
{
	//assert1(m_RootElement != nullptr);
	//delete m_RootElement;

    //

	delete m_FontsMgr;

	DelManager<IUIMgr>();

	_Bindings->UnregisterInputListener(this);
	_Bindings->UnregisterRenderableUIObject(this);
	_Bindings->UnregisterUpdatableObject(this);
}

//

void UIMgr::AttachToRoot(UIElement* _element)
{
    assert1(m_RootElement != nullptr);
	if (m_RootElement != nullptr)
	{
		AttachElementToParent(_element, m_RootElement);
	}
}

void UIMgr::DetachFromRoot(UIElement * _element, bool _checkChilds)
{

}

void UIMgr::AttachElementToParent(UIElement* _element, UIElement* _parent)
{
    // If element already has a parent
    if (_element->m_Parent != nullptr)
    {
        Log::Error("UIElement: Can't attach element [%s], because element already have a parent [%s]!", _element->GetName().c_str(), _element->m_Parent->GetName().c_str());
        return;
    }

    // Attach to root case
    if (_parent == nullptr)
    {
        AttachToRoot(_element);
        return;
    }

    // Add element in parent childs
    _parent->m_Childs.push_back(_element);

    // Set element parent
    _element->m_Parent = _parent;
    _element->m_Deep = _parent->m_Deep + 1;
    _element->m_Deep += _element->m_DeepAdding;
}

//

void UIMgr::SetFocus(UIElement* _element) 
{
    assert1(_element != nullptr);

    if (_element->m_Deep > m_FocusedElement->m_Deep)
    {
        m_FocusedElement = _element;
    }
}

//

void UIMgr::RenderImage(vec2 _pos, Image* _image)
{
	RenderImage(_pos, _image, _image->GetSize());
}

void UIMgr::RenderImage(vec2 _pos, Image* _image, vec2 _size)
{
	// Transform
	mat4 worldTransform;
	worldTransform.translate(_pos.x + _size.x / 2.0f, _pos.y + _size.y / 2.0f, 0.0f);
	worldTransform.scale(_size.x / 2.0f, _size.y / 2.0f, 1.0f);

	// Update buffer
	vector<vec2> texCoordsQuad;
	texCoordsQuad.push_back(_image->GetP0());
	texCoordsQuad.push_back(_image->GetP1());
	texCoordsQuad.push_back(_image->GetP3());
	texCoordsQuad.push_back(_image->GetP2());
	_Render->getRenderStorage()->__vbQuadVTDynamic->updateBufferData(4 * sizeof(vec3), 4 * sizeof(vec2), texCoordsQuad.data());

	// Shader
	_Render->getTechniquesMgr()->UI_Texture->Bind();
	_Render->getTechniquesMgr()->UI_Texture->setProj(_Render->getOrthoMatrix() * worldTransform);

	// State
	_Render->r.setTexture(Material::C_DiffuseTextureIndex, _image->GetTexture(), SS_FILTER_BILINEAR | SS_ANISO16 | SS_ADDR_CLAMP, 0);
	_Render->r.setGeometry(_Render->getRenderStorage()->__QuadVTDynamic);

	// Draw call
	_Render->r.drawIndexed(0, 6, 0, 4);

	_Render->getTechniquesMgr()->UI_Texture->Unbind();
}

void UIMgr::RenderText(vec2 _pos, cstring _string, const Color& _color) const
{
	RenderText(_pos, _string, TextAlignW::TEXT_ALIGNW_LEFT, TextAlignH::TEXT_ALIGNH_BOTTOM, m_FontsMgr->GetMainFont(), _color);
}

void UIMgr::RenderText(vec2 _pos, cstring _string, Font* _font, const Color& _color) const
{
	RenderText(_pos, _string, TextAlignW::TEXT_ALIGNW_LEFT, TextAlignH::TEXT_ALIGNH_BOTTOM, _font, _color);
}

void UIMgr::RenderText(vec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, const Color& _color) const
{
	RenderText(_pos, _string, _alignW, _alignH, m_FontsMgr->GetMainFont(), _color);
}

void UIMgr::RenderText(vec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, Font* _font, const Color& _color) const
{
	auto stringWidth = _font->GetStringWidth(_string);
	auto fontHeight = _font->GetHeight();

	vec2 offset = vec2();

	switch (_alignW)
	{
	case TEXT_ALIGNW_LEFT:
		offset.x = 0;
		break;

	case TEXT_ALIGNW_CENTER:
		offset.x = -static_cast<float>(stringWidth / 2);
		break;

	case TEXT_ALIGNW_RIGHT:
		offset.x = -static_cast<float>(stringWidth);
		break;
	}

	switch (_alignH)
	{
	case TEXT_ALIGNH_TOP:
		offset.y = -static_cast<float>(fontHeight);
		break;

	case TEXT_ALIGNH_CENTER:
		offset.y = -static_cast<float>(fontHeight / 2);
		break;

	case TEXT_ALIGNH_BOTTOM:
		offset.y = 0;
		break;
	}

	_font->Render(_string, _pos + offset, _color);
}


//


void UIMgr::Update(double _time, double _dTime)
{
	// Detach from parent
	for (auto it = m_ObjectsToDetach.begin(); it != m_ObjectsToDetach.end(); )
	{
		DetachFromParent(*it);

		it = m_ObjectsToDetach.erase(it);
	}

	// Delete
	for (auto it = m_ObjectsToDelete.begin(); it != m_ObjectsToDelete.end(); )
	{
		DetachFromParent(*it);

		(*it)->DeleteChilds();

		Log::Info("UI: Element [%s] deleted", (*it)->GetName().c_str());
		DeleteUIElement(*it);

		it = m_ObjectsToDelete.erase(it);
	}

	// Update window
	assert1(m_RootElement != nullptr);
	m_FocusedElement = m_RootElement;
	m_RootElement->Update();
}

void UIMgr::RenderUI()
{
    // Update window
    assert1(m_RootElement != nullptr);
	if (m_RootElement != nullptr)
	{
		m_RootElement->RenderUI();
	}

    // Render debug
    /*if (m_FocusedElement != nullptr)
    {
        _Render->RenderRectangleOutline(m_FocusedElement->GetPosition(), m_FocusedElement->GetSize(), COLOR_BLUE);
    }*/
}

//

string UIMgr::GetNewName()
{
	string name = "UIElement_" + to_string(m_IDCounter);
	m_IDCounter++;
	return name;
}

void UIMgr::SetForDetach(UIElement* _element)
{
	if (find(m_ObjectsToDetach.begin(), m_ObjectsToDetach.end(), _element) != m_ObjectsToDetach.end())
	{
		Log::Warn("UI: Element [%s] already set for detaching.", _element->GetName().c_str());
		return;
	}

	m_ObjectsToDetach.push_back(_element);
}

void UIMgr::SetForDelete(UIElement* _element)
{
	if (find(m_ObjectsToDelete.begin(), m_ObjectsToDelete.end(), _element) != m_ObjectsToDelete.end())
	{
		Log::Warn("UI: Element [%s] already set for deletion.", _element->GetName().c_str());
		return;
	}

	m_ObjectsToDelete.push_back(_element);
}

void UIMgr::DetachFromParent(UIElement* _element)
{
	auto& parent = _element->m_Parent;

	if (parent == nullptr)
	{
		Log::Error("UI: Element [%s] parent is nullptr.", _element->GetName().c_str());
		return;
	}

	auto& elementInParentChildsIt = find(parent->m_Childs.begin(), parent->m_Childs.end(), _element);
	if (*elementInParentChildsIt != _element)
	{
		Log::Error("UI: Element [%s] not finded in parent [%s] childs.", _element->GetName().c_str(), parent->GetName().c_str());
		return;
	}

	parent->m_Childs.erase(elementInParentChildsIt);
	Log::Info("UI: Element [%s] detached from parent [%s].", _element->GetName().c_str(), parent->GetName().c_str());
}

void UIMgr::DeleteUIElement(UIElement* _element)
{
	if (m_FocusedElement == _element)
	{
		m_FocusedElement = nullptr;
	}

	delete _element;
	_element = nullptr;
}

//

#pragma region Input functional

void UIMgr::OnMouseMoved(cvec2 _mousePos)
{
	if (m_RootElement != nullptr)
	{
		m_RootElement->OnMouseMoved(_mousePos);
	}
}

bool UIMgr::OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos)
{
    if (m_FocusedElement != nullptr)
    {
        return m_FocusedElement->OnMouseButtonPressed(_button, _mods, _mousePos);
    }

	return false;
}

bool UIMgr::OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos)
{
    assert1(m_RootElement != nullptr);
    return m_RootElement->OnMouseButtonReleased(_button, _mods, _mousePos);
}

bool UIMgr::OnMouseWheel(int _yoffset)
{
    assert1(m_RootElement != nullptr);
    return m_RootElement->OnMouseWheel(_yoffset);
}

bool UIMgr::OnKeyboardPressed(int _key, int _scancode, int _mods)
{
    assert1(m_RootElement != nullptr);
    return m_RootElement->OnKeyboardPressed(_key, _scancode, _mods);
}

bool UIMgr::OnKeyboardReleased(int _key, int _scancode, int _mods)
{
    assert1(m_RootElement != nullptr);
    return m_RootElement->OnKeyboardReleased(_key, _scancode, _mods);
}

bool UIMgr::OnCharInput(uint32 _char)
{
    if (m_FocusedElement != nullptr)
    {
        return m_FocusedElement->OnCharInput(_char);
    }

    return false;
}

#pragma endregion
