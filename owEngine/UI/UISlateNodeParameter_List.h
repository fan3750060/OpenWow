#pragma once

// Common UI
#include "UIContainerListWithActivation.h"

// Slate
#include "UISlateNodeParameter.h"

class CUISlateNodeParameter_List : public CUISlateNodeParameter
{
    typedef CUISlateNodeParameter base;
public:
    CUISlateNodeParameter_List(std::weak_ptr<CUISlateEditor> Editor);
    virtual ~CUISlateNodeParameter_List();

    // CUISlateNodeParameter_List
    void Initialize(const std::vector<std::string>& List);

    // CUIBaseNode
    virtual glm::vec2 GetSize() const override;
    virtual std::vector<std::shared_ptr<CUIBaseNode>> GetChilds() const override;

public:
    void OnListHeaderClicked(UIBaseNodeClickedEventArgs& e);

private:
    std::shared_ptr<CUITextNode>                    m_Text;
    std::shared_ptr<CUIContainerListWithActivation> m_ListWithActivation;

    std::vector<std::string> m_List;

};