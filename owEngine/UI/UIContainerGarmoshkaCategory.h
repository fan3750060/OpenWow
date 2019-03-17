#pragma once

// Common UI
#include "UIBaseNode.h"

// ContainerGarmoshka
#include "UIContainerGarmoshkaCategoryHeader.h"

// FORWARD BEGIN
class CUIContainerGarmoshka;
// FORWARD END

class CUIContainerGarmoshkaCategory : public CUIBaseNode
{
public:
    CUIContainerGarmoshkaCategory(std::weak_ptr<CUIContainerGarmoshka> ContainerGarmoshka);
    virtual ~CUIContainerGarmoshkaCategory();

    // CUIContainerGarmoshkaCategory
    void Initialize(const std::string& CategoryName);
    void CreateDefault();

    void AddChild(std::shared_ptr<CUIBaseNode> Child);
    void SetCategoryName(cstring Name);
    void SetActive(bool Active);

    // CUIBaseNode
    glm::vec2 GetSize() const override final;
    std::vector<std::shared_ptr<CUIBaseNode>> GetChilds() const override final;

protected:
    void CalculateChildsTranslate();

private:
    std::shared_ptr<CUIContainerGarmoshkaCategoryHeader>    m_Header;
    std::vector<std::shared_ptr<CUIBaseNode>>               m_Nodes;
    bool                                                    m_IsActive;

    // Weak refs
    std::weak_ptr<CUIContainerGarmoshka>                    m_ContainerGarmoshka;
};