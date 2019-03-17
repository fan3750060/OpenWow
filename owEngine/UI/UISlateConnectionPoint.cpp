#include "stdafx.h"

// General
#include "UISlateConnectionPoint.h"

// Additional
#include "Application.h"
#include "UISlateEditor.h"
#include "UISlateNode.h"


namespace
{
    // Line point 
    const float  cLinePointIconSize = 12.0f;
    const vec4   cLinePointOpenedColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    const vec4   cLinePointClosedColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}


CUISlateConnectionPoint::CUISlateConnectionPoint(std::weak_ptr<CUISlateEditor> Editor)
    : base(glm::vec2(cLinePointIconSize, cLinePointIconSize))
    , m_Editor(Editor)
{
}

CUISlateConnectionPoint::~CUISlateConnectionPoint()
{
}



//
// CUISlateConnectionPoint
//
void CUISlateConnectionPoint::Initialize()
{
    SetTexture(_RenderDevice->CreateTexture2D("Textures\\slate_round_filled.png"));
}



//
// Input events
//
bool CUISlateConnectionPoint::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
    std::shared_ptr<CUISlateEditor> editor = m_Editor.lock();
    _ASSERT(editor);

    std::shared_ptr<CUIBaseNode> parent = m_pParentNode.lock();
    _ASSERT(parent);

    std::shared_ptr<IUISlateConnectionable> parentAsConnectionable = std::dynamic_pointer_cast<IUISlateConnectionable>(parent);
    _ASSERT(parentAsConnectionable);

    editor->BeginMakeConnection(parentAsConnectionable);

    return true;
}

void CUISlateConnectionPoint::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
    std::shared_ptr<CUISlateEditor> editor = m_Editor.lock();
    _ASSERT(editor);

    std::shared_ptr<CUIBaseNode> parent = m_pParentNode.lock();
    _ASSERT(parent);

    std::shared_ptr<IUISlateConnectionable> parentAsConnectionable = std::dynamic_pointer_cast<IUISlateConnectionable>(parent);
    _ASSERT(parentAsConnectionable);

    if (parentAsConnectionable->GetConnectRectangle().isPointInside(e.GetPoint()))
    {
        editor->FinishMakeConnection(parentAsConnectionable);
    }
}
