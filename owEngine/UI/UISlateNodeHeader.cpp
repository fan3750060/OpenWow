#include "stdafx.h"

// General
#include "UISlateNodeHeader.h"

// Additional
#include "Application.h"
#include "UISlateEditor.h"
#include "UISlateNode.h"


namespace
{
	// Background
	const vec2  cHeaderBackgroundSize  = vec2(240.0f, 56.0f);
	const vec4  cHeaderBackgroundColor = vec4(0.33f, 0.33f, 0.33f, 1.0f);

	// Icon
	const vec2  cHeaderIconOffset = vec2(10.0f, 10.0f);
	const float cHeaderIconSize   = 32.0f;

	// Text
    const char*  cHeaderTextFontPath = "Fonts\\JustBreatheBd.otf";
	const uint32 cHeaderTextHeight = 16;
	const vec2   cHeaderTextOffset = vec2(5.0f, 0.0f);
	const vec4   cHeaderTextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}


CUISlateNodeHeader::CUISlateNodeHeader(std::weak_ptr<CUISlateEditor> Editor)
    : m_Editor(Editor)
{

}

CUISlateNodeHeader::~CUISlateNodeHeader()
{
}



//
// CUISlateNodeHeader
//
void CUISlateNodeHeader::Initialize(const std::string& HeaderText, const std::string& HeaderIconPath)
{
    // Header background
    m_Background = std::make_shared<CUITextureNode>(cHeaderBackgroundSize);
    m_Background->SetParentInternal(weak_from_this());

    std::shared_ptr<Texture> backgroundTexture = _RenderDevice->CreateTexture2D("Textures\\Slate\\slate_header.png");
    m_Background->SetTexture(backgroundTexture);
    
    // Icon (might not be presented)
    CreateIconIfNeed(HeaderIconPath);

    // Header text (right to icon)
    m_Text = std::make_shared<CUITextNode>();
    m_Text->SetParentInternal(weak_from_this());
    m_Text->SetText(HeaderText);
    m_Text->SetTextColor(cHeaderTextColor);
    m_Text->SetFont(GetManager<IFontsManager>()->Add(cHeaderTextFontPath, cHeaderTextHeight));

    CalculateChildsTranslate();
}

void CUISlateNodeHeader::SetHeaderText(const std::string & HeaderText)
{
    _ASSERT(m_Text != nullptr);

    m_Text->SetText(HeaderText);
}

void CUISlateNodeHeader::SetHeaderIcon(const std::string & HeaderIconPath)
{
    CreateIconIfNeed(HeaderIconPath);
}



//
// CUIBaseNode
//

glm::vec2 CUISlateNodeHeader::GetSize() const
{
    return m_Background->GetSize();
}

std::vector<std::shared_ptr<CUIBaseNode>> CUISlateNodeHeader::GetChilds() const
{
    std::vector<std::shared_ptr<CUIBaseNode>> childs;

    if (m_Background)
        childs.push_back(m_Background);

    if (m_IconNode)
        childs.push_back(m_IconNode);

    if (m_Text)
        childs.push_back(m_Text);

    return childs;
}



//
// Input events
//
bool CUISlateNodeHeader::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
    std::shared_ptr<CUISlateEditor> editor = m_Editor.lock();
    _ASSERT(editor);

    std::shared_ptr<CUISlateNode> parentAsSlate = std::dynamic_pointer_cast<CUISlateNode>(GetParent());
    _ASSERT(parentAsSlate);

    editor->BeginMoveNode(parentAsSlate, e.GetPoint() - GetParent()->GetTranslationAbs());

    return true;
}

void CUISlateNodeHeader::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
}





//
// Protected
//

void CUISlateNodeHeader::CreateIconIfNeed(const std::string& HeaderIconPath)
{
    if (HeaderIconPath.empty())
        return;

    if (!GetManager<IFilesManager>()->IsFileExists(HeaderIconPath))
        return;

    std::shared_ptr<Texture> iconTexture = _RenderDevice->CreateTexture2D(HeaderIconPath);
    _ASSERT(iconTexture);

    if (m_IconNode == nullptr)
    {
        m_IconNode = std::make_shared<CUITextureNode>(vec2(cHeaderIconSize, cHeaderIconSize));
        m_IconNode->SetParentInternal(weak_from_this());
    }

    m_IconNode->SetTexture(iconTexture);

    CalculateChildsTranslate();
}

void CUISlateNodeHeader::CalculateChildsTranslate()
{
    glm::vec2 headerTextTranslate = cHeaderTextOffset;

    // Header
    // don't touch it position

    // Icon
    if (m_IconNode)
    {
        m_IconNode->SetTranslate(cHeaderIconOffset);

        headerTextTranslate += m_IconNode->GetTranslation();
        headerTextTranslate += glm::vec2(m_IconNode->GetSize().x, 0.0f);
    }

    // Text (right to icon)
    if (m_Text)
        m_Text->SetTranslate(headerTextTranslate);
}
