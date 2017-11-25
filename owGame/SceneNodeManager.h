#pragma once

#include "SceneNode.h"

class SceneNodeManager
{
public:
    SceneNodeManager();
    ~SceneNodeManager();

    static SceneNode* Root();

private:
    static SceneNode* m_RootNode;
};