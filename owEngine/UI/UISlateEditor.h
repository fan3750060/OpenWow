#pragma once

// Common UI
#include "UIBaseNode.h"
#include "UIColor.h"
#include "UIBeizerLine.h"

// Slate
#include "UISlateEditorInterfaces.h"
#include "UISlateNode.h"
#include "UISlateConnection.h"

class CUISlateEditor : public CUIBaseNode
{
    typedef CUIBaseNode base;
public:
    CUISlateEditor();
    virtual ~CUISlateEditor();


    //
    // CUISlateEditor
    //
    void Initialize(vec2 Size);

    // Add / remove slate nodes
    void AddNode(std::shared_ptr<CUISlateNode> Node);
    void RemoveNode(std::shared_ptr<CUISlateNode> Node);

    // Connect / disconnect result of one slate node to parameter from another node
    void BeginMakeConnection(std::shared_ptr<IUISlateConnectionable> Initiator);
    void FinishMakeConnection(std::shared_ptr<IUISlateConnectionable> Target);

    // Moving nodes
    void BeginMoveNode(std::shared_ptr<CUISlateNode> Node, glm::vec2 Point);


    //
    // CUIBaseNode
    //
    virtual std::vector<std::shared_ptr<CUIBaseNode>> GetChilds() const override;

    virtual bool Accept(IVisitor& visitor);

    // Input events
    virtual void OnMouseMoved(MouseMotionEventArgs& e) override;
    virtual bool OnMouseButtonPressed(MouseButtonEventArgs& e) override;
    virtual void OnMouseButtonReleased(MouseButtonEventArgs& e) override;
    virtual bool OnMouseWheel(MouseWheelEventArgs& e) override;

protected:
    void MakeConnection(std::shared_ptr<IUISlateConnectionable> Initiator, std::shared_ptr<IUISlateConnectionable> Target);

private:
    std::shared_ptr<CUIColorNode>               m_Background;

    // Slate nodes
    typedef std::vector<std::shared_ptr<CUISlateNode>> NodeList;
    typedef std::unordered_multimap<std::string, std::shared_ptr<CUISlateNode>> NodeNameMap;

    NodeList                                    m_Nodes;
    NodeNameMap                                 m_NodesByName;

    // Connections
    std::shared_ptr<CUIBeizerLineNode>          m_CurrentLine;
    std::shared_ptr<IUISlateConnectionable>     m_CurrentConnectionable;

    typedef std::vector<std::shared_ptr<CUISlateConnection>> ConnectionsList;
    ConnectionsList                             m_Connections;

    // Move nodes
    std::shared_ptr<CUISlateNode>               m_MovingNodes_Current;
    glm::vec2                                   m_MovingNodes_Point;
};