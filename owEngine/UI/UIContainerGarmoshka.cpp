#include "stdafx.h"

// General
#include "UIContainerGarmoshka.h"

CUIContainerGarmoshka::CUIContainerGarmoshka()
{
}

CUIContainerGarmoshka::~CUIContainerGarmoshka()
{
}



//
// CUIContainerGarmoshka
//
void CUIContainerGarmoshka::CreateDefault()
{
    std::shared_ptr<CUIContainerGarmoshkaCategory> category1 = std::make_shared<CUIContainerGarmoshkaCategory>(std::dynamic_pointer_cast<CUIContainerGarmoshka>(shared_from_this()));
    category1->SetParentInternal(weak_from_this());
    category1->CreateDefault();
    m_Categories.insert(std::make_pair("catName1", category1));

    std::shared_ptr<CUIContainerGarmoshkaCategory> category2 = std::make_shared<CUIContainerGarmoshkaCategory>(std::dynamic_pointer_cast<CUIContainerGarmoshka>(shared_from_this()));
    category2->SetParentInternal(weak_from_this());
    category2->CreateDefault();
    m_Categories.insert(std::make_pair("catName2", category2));

    CalculateCategoriesTranslate(vec2(), vec2(0.0f, 15.0f));
}



//
// CUIBaseNode
//
std::vector<std::shared_ptr<CUIBaseNode>> CUIContainerGarmoshka::GetChilds() const
{
    std::vector<std::shared_ptr<CUIBaseNode>> childs;

    for (auto category : m_Categories)
        childs.push_back(category.second);

    return childs;
}



//
// Input events
bool CUIContainerGarmoshka::OnMouseButtonPressed(MouseButtonEventArgs & e)
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

void CUIContainerGarmoshka::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
    for (auto it : GetChilds())
    {
        it->OnMouseButtonReleased(e);
    }
}



//
// Protected
//
void CUIContainerGarmoshka::CalculateCategoriesTranslate(glm::vec2 StartPosition, glm::vec2 EveryNodeOffset)
{
    glm::vec2 increment = StartPosition;

    for (auto categoryIter : m_Categories)
    {
        std::shared_ptr<CUIContainerGarmoshkaCategory> category = categoryIter.second;

        category->SetTranslate(EveryNodeOffset + increment);
        increment += vec2(0.0f, EveryNodeOffset.y);
        increment += vec2(0.0f, category->GetSize().y);
    }
}
