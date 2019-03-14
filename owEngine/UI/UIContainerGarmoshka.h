#pragma once

// Common UI
#include "UIBaseNode.h"

// ContainerGarmoshka
#include "UIContainerGarmoshkaCategory.h"

class CUIContainerGarmoshka : public CUIBaseNode
{
public:
    CUIContainerGarmoshka();
    virtual ~CUIContainerGarmoshka();

    // CUIContainerGarmoshka
    void CreateDefault();

    // CUIBaseNode
    std::vector<std::shared_ptr<CUIBaseNode>> GetChilds() const override;

    // Input events
    virtual bool OnMouseButtonPressed(MouseButtonEventArgs& e) override;
    virtual void OnMouseButtonReleased(MouseButtonEventArgs& e) override;

protected:
    void CalculateCategoriesTranslate(glm::vec2 StartPosition, glm::vec2 EveryNodeOffset);

private:
    std::unordered_multimap<std::string, std::shared_ptr<CUIContainerGarmoshkaCategory>> m_Categories;

};