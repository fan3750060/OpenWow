#pragma once

// Common UI
#include "UIBaseNode.h"
#include "UIColor.h"
#include "UIText.h"

class CUIContainerGarmoshkaCategoryHeader : public CUIBaseNode
{
public:
    CUIContainerGarmoshkaCategoryHeader();
    virtual ~CUIContainerGarmoshkaCategoryHeader();

    // CUIContainerGarmoshkaCategoryHeader
    void CreateDefault();
    void SetText(cstring Text);

    // CUIBaseNode
    glm::vec2 GetSize() const override final;

    std::vector<std::shared_ptr<CUIBaseNode>> GetChilds() const override final;

    // Input events
    virtual bool OnMouseButtonPressed(MouseButtonEventArgs& e) override final;
    virtual void OnMouseButtonReleased(MouseButtonEventArgs& e) override final;

private:
    std::shared_ptr<CUIColorNode>  m_Background;
    std::shared_ptr<CUITextNode>   m_Text;
};