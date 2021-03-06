#pragma once

// FORWARD BEGIN
class CM2_Base_Instance;
// FORWARD END

class 
    __declspec(UUID_ColliderComponent)
    CM2_ColliderComponent : public CColliderComponent
{
    typedef CColliderComponent base;
public:
    CM2_ColliderComponent(std::shared_ptr<SceneNode3D> OwnerNode);
    virtual ~CM2_ColliderComponent();

protected:
    std::shared_ptr<CM2_Base_Instance> GetOwnerNode();

    // CColliderComponent
    virtual void UpdateBounds() override;
};