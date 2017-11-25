#include "stdafx.h"

// General
#include "SceneNodeManager.h"

SceneNodeManager::SceneNodeManager()
{
    m_RootNode = new SceneNode(SceneNode::SceneNodeType::Root);
}

SceneNodeManager::~SceneNodeManager()
{}
