#include "stdafx.h"

// General
#include "UIContainerGarmoshkaCategory.h"

// Additional
#include "UIContainerGarmoshka.h"

#include "UIUtils.h"
#include "UIText.h"


CUIContainerGarmoshkaCategory::CUIContainerGarmoshkaCategory(std::weak_ptr<CUIContainerGarmoshka> ContainerGarmoshka)
    : m_ContainerGarmoshka(ContainerGarmoshka)
{
}

CUIContainerGarmoshkaCategory::~CUIContainerGarmoshkaCategory()
{
}

void CUIContainerGarmoshkaCategory::CreateDefault()
{
    m_Header = std::make_shared<CUIContainerGarmoshkaCategoryHeader>();
    m_Header->SetParentInternal(weak_from_this());
    m_Header->CreateDefault();
    m_Header->SetText("Some header text");

    glm::vec2 textOffset = glm::vec2(5.0f, 5.0f);

    std::shared_ptr<CUITextNode> ch1 = std::make_shared<CUITextNode>();
    ch1->SetParentInternal(weak_from_this());
    ch1->SetText("Some child #1");
    ch1->SetOffset(textOffset);
    m_Nodes.push_back(ch1);


    std::shared_ptr<CUITextNode> ch2 = std::make_shared<CUITextNode>();
    ch2->SetParentInternal(weak_from_this());
    ch2->SetText("Some child #2");
    ch2->SetOffset(textOffset);
    m_Nodes.push_back(ch2);


    std::shared_ptr<CUITextNode> ch3 = std::make_shared<CUITextNode>();
    ch3->SetParentInternal(weak_from_this());
    ch3->SetText("Some child #3");
    ch3->SetOffset(textOffset);
    m_Nodes.push_back(ch3);

    CalculateNodesTranslate(vec2(0.0, m_Header->GetSize().y));
}

void CUIContainerGarmoshkaCategory::SetText(cstring Name)
{
    m_Header->SetText(Name);
}



//
// CUIBaseNode
//
glm::vec2 CUIContainerGarmoshkaCategory::GetSize() const
{
    return owUIUtils::CalculateChildsVerticalSize(GetChilds(), 26.0f);
}

std::vector<std::shared_ptr<CUIBaseNode>> CUIContainerGarmoshkaCategory::GetChilds() const
{
    std::vector<std::shared_ptr<CUIBaseNode>> childs;

    if (m_Header)
        childs.push_back(m_Header);

    for (auto node : m_Nodes)
        childs.push_back(node);

    return childs;
}

bool CUIContainerGarmoshkaCategory::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
    bool result = false;
    for (auto it : GetChilds())
    {
        if (it->IsPointInBoundsAbs(e.GetPoint()))
            if (it->OnMouseButtonPressed(e))
                result = true;
    }
    return result;
}

void CUIContainerGarmoshkaCategory::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
    for (auto it : GetChilds())
    {
        it->OnMouseButtonReleased(e);
    }
}



//
// Protected
//
void CUIContainerGarmoshkaCategory::CalculateNodesTranslate(glm::vec2 StartPosition, glm::vec2 EveryNodeOffset)
{
    glm::vec2 increment = StartPosition;

    for (auto node : m_Nodes)
    {
        node->SetTranslate(EveryNodeOffset + increment);
        increment += vec2(0.0f, EveryNodeOffset.y);
        increment += vec2(0.0f, node->GetSize().y);
    }
}
