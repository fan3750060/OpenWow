#pragma once

// Common UI
#include "UIBaseNode.h"
#include "UITexture.h"

// Slate
#include "UISlateEditorInterfaces.h"

// FORWARD BEGIN
class CUISlateEditor;
class CUISlateNode;
// FORWARD END

class CUISlateConnectionPoint : public CUITextureNode
{
    typedef CUITextureNode base;
public:
    enum ConnectionState : uint32
    {
        Disconnected = 0,
        Connected,
        AcceptConnection
    };
public:
    CUISlateConnectionPoint(std::weak_ptr<CUISlateEditor> Editor);
    virtual ~CUISlateConnectionPoint();

    // CUISlateConnectionPoint
    void Initialize();

    // CUITextureNode
    // no overrides

    // Input events
    bool OnMouseButtonPressed(MouseButtonEventArgs& e) override;
    void OnMouseButtonReleased(MouseButtonEventArgs& e) override;

private:
    ConnectionState                     m_ConnectionState;

    // Weak refs
    std::weak_ptr<CUISlateEditor>       m_Editor;
};