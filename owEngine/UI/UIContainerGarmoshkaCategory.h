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
    void CreateDefault();
    void SetText(cstring Name);

    // CUIBaseNode
    glm::vec2 GetSize() const override final;

    std::vector<std::shared_ptr<CUIBaseNode>> GetChilds() const override final;

    // Input events
    virtual bool OnMouseButtonPressed(MouseButtonEventArgs& e) override;
    virtual void OnMouseButtonReleased(MouseButtonEventArgs& e) override;

protected:
    void CalculateNodesTranslate(glm::vec2 StartPosition, glm::vec2 EveryNodeOffset = glm::vec2(0.0f, 0.0f));

private:
    std::shared_ptr<CUIContainerGarmoshkaCategoryHeader>    m_Header;
    std::vector<std::shared_ptr<CUIBaseNode>>               m_Nodes;

    // Weak refs
    std::weak_ptr<CUIContainerGarmoshka>                    m_ContainerGarmoshka;
};