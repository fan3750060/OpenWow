#pragma once

// Base
#include "UIBaseNode.h"

// Childs
#include "UIColor.h"
#include "UIText.h"
#include "UITexture.h"

// FORWARD BEGIN
class CUISlateNode;
// FORWARD END

class CUISlateNodeHeader : public CUIBaseNode
{
    typedef CUIBaseNode base;
public:
    CUISlateNodeHeader();
    virtual ~CUISlateNodeHeader();

    // CUISlateNodeHeader
    void CreateDefault();

    // CUIBaseNode
    virtual bool Accept(IVisitor& visitor) override;
    virtual bool AcceptMesh(IVisitor& visitor) override;

private:
    std::shared_ptr<CUIColorNode>   m_BackgroungNode;
    std::shared_ptr<CUITextureNode> m_IconNode;
    std::shared_ptr<CUITextNode>    m_LabelNode;
};