#include "stdafx.h"

// General
#include "UIContainerGarmoshkaCategory.h"

// Additional
#include "UIContainerGarmoshka.h"

#include "UIUtils.h"
#include "UIText.h"

namespace
{
    // Icon
    const vec2  cChildsOffset          = vec2(5.0f, 5.0f);
    const float cChildsVerticalPadding = 2.0f;
}

CUIContainerGarmoshkaCategory::CUIContainerGarmoshkaCategory(std::weak_ptr<CUIContainerGarmoshka> ContainerGarmoshka)
    : m_IsActive(false)
    , m_ContainerGarmoshka(ContainerGarmoshka)
{
}

CUIContainerGarmoshkaCategory::~CUIContainerGarmoshkaCategory()
{
}



//
// CUIContainerGarmoshkaCategory
//
void CUIContainerGarmoshkaCategory::Initialize(const std::string& CategoryName)
{
    m_Header = std::make_shared<CUIContainerGarmoshkaCategoryHeader>(m_ContainerGarmoshka);
    m_Header->SetParentInternal(weak_from_this());
    m_Header->Initialize();
    m_Header->SetText(CategoryName);
}

void CUIContainerGarmoshkaCategory::CreateDefault()
{
    const glm::vec2 textOffset = glm::vec2(5.0f, 5.0f);

    std::shared_ptr<CUITextNode> ch1 = std::make_shared<CUITextNode>();
    ch1->SetText("Some child #1");
    ch1->SetTextColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));
    ch1->SetOffset(textOffset);
    AddChild(ch1);

    std::shared_ptr<CUITextNode> ch2 = std::make_shared<CUITextNode>();
    ch2->SetText("Some child #2");
    ch2->SetTextColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));
    ch2->SetOffset(textOffset);
    AddChild(ch2);

    std::shared_ptr<CUITextNode> ch3 = std::make_shared<CUITextNode>();
    ch3->SetText("Some child #3");
    ch3->SetTextColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));
    ch3->SetOffset(textOffset);
    AddChild(ch3);

    std::shared_ptr<CUITextNode> ch4 = std::make_shared<CUITextNode>();
    ch4->SetText("Some child #4");
    ch4->SetTextColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));
    ch4->SetOffset(textOffset);
    AddChild(ch4);

    std::shared_ptr<CUITextNode> ch5 = std::make_shared<CUITextNode>();
    ch5->SetText("Some child #5");
    ch5->SetTextColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));
    ch5->SetOffset(textOffset);
    AddChild(ch5);
}

void CUIContainerGarmoshkaCategory::AddChild(std::shared_ptr<CUIBaseNode> Child)
{
    _ASSERT(Child != nullptr);

    Child->SetParentInternal(weak_from_this());
    m_Nodes.push_back(Child);

    CalculateChildsTranslate();
}

void CUIContainerGarmoshkaCategory::SetCategoryName(cstring Name)
{
    m_Header->SetText(Name);
}

void CUIContainerGarmoshkaCategory::SetActive(bool Active)
{
    m_IsActive = Active;
}



//
// CUIBaseNode
//
glm::vec2 CUIContainerGarmoshkaCategory::GetSize() const
{
    return owUIUtils::CalculateChildsVerticalSize(GetChilds(), cChildsVerticalPadding);
}

std::vector<std::shared_ptr<CUIBaseNode>> CUIContainerGarmoshkaCategory::GetChilds() const
{
    std::vector<std::shared_ptr<CUIBaseNode>> childs;

    if (m_Header)
        childs.push_back(m_Header);

    if (m_IsActive)
        for (auto node : m_Nodes)
            childs.push_back(node);

    return childs;
}



//
// Protected
//
void CUIContainerGarmoshkaCategory::CalculateChildsTranslate()
{
    glm::vec2 currentTranslate = m_Header->GetTranslation() + vec2(0.0, m_Header->GetSize().y);

    for (auto node : m_Nodes)
    {
        node->SetTranslate(cChildsOffset + currentTranslate);

        currentTranslate += vec2(0.0f, node->GetSize().y);
        currentTranslate += vec2(0.0f, cChildsVerticalPadding);
    }
}
