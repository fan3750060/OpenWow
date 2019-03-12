#pragma once

// Base
#include "UIBaseNode.h"

// Childs
#include "UISlateNodeHeader.h"
#include "UISlateNodeParameter.h"
#include "UISlateNodeFooter.h"

class CUISlateNode : public CUIBaseNode
{
	typedef CUIBaseNode base;
public:
	CUISlateNode();
	virtual ~CUISlateNode();

	// CUISlateNode
	void CreateDefault();

    // Content
    void SetHeader(std::shared_ptr<CUISlateNodeHeader> Header);
    void AddParameter(std::shared_ptr<CUISlateNodeParameter> Parameter);
    void RemoveParameter(std::shared_ptr<CUISlateNodeParameter> Parameter);
    void SetFooter(std::shared_ptr<CUISlateNodeFooter> Footer);

	// CUIBaseNode
    void SetParent(std::weak_ptr<CUIBaseNode> parent) override;         // Call this from CUIWindowNode

	virtual bool Accept(IVisitor& visitor) override;
	virtual bool AcceptMesh(IVisitor& visitor) override;

private:
    typedef std::unordered_multimap<std::string, std::shared_ptr<CUISlateNodeParameter>> SlateParameterNameMap;

	std::shared_ptr<CUISlateNodeHeader>   m_Header;
    SlateParameterNameMap                 m_Parameters;
    std::shared_ptr<CUISlateNodeFooter>   m_Footer;
};