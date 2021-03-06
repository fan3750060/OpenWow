#include "stdafx.h"

// Include
#include "M2_Base_Instance.h"

// General
#include "M2_TransformComponent.h"

CM2_TransformComponent::CM2_TransformComponent(std::shared_ptr<SceneNode3D> OwnerNode)
    : CTransformComponent(OwnerNode)
{
}

CM2_TransformComponent::~CM2_TransformComponent()
{
}

std::shared_ptr<CM2_Base_Instance> CM2_TransformComponent::GetOwnerNode()
{
    return std::dynamic_pointer_cast<CM2_Base_Instance, SceneNode3D>(base::GetOwnerNode());
}



//
// CTransformComponent
//
void CM2_TransformComponent::UpdateLocalTransform()
{
    std::shared_ptr<CM2_Part_Attachment> attachPoint = GetOwnerNode()->GetAttachPoint();

    if (attachPoint)
    {
        std::shared_ptr<const CM2_Part_Bone> bone = attachPoint->getBone().lock();
        assert1(bone != nullptr);

        mat4 relMatrix;
        relMatrix = glm::translate(relMatrix, bone->getPivot());

        mat4 absMatrix;
        absMatrix = GetParentWorldTransform() * bone->getTransformMatrix() * relMatrix;
        SetWorldTransform(absMatrix);
    }
    else
    {
        base::UpdateLocalTransform();
    }
}