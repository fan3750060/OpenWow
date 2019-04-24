#include "stdafx.h"

// General
#include "LiquidInstance.h"

Liquid_Instance::Liquid_Instance() :
	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{}

Liquid_Instance::~Liquid_Instance()
{
}

void Liquid_Instance::Initialize(std::shared_ptr<CLiquid> _liquidObject, vec3 _position)
{
    // Transform
    std::shared_ptr<CTransformComponent> transform = GetComponent<CTransformComponent>();
    {
        transform->SetTranslate(_position);
    }

    // Collider
    std::shared_ptr<CColliderComponent> collider = GetComponent<CColliderComponent>();
    {
        BoundingBox bbox(vec3(Math::MinFloat, Math::MinFloat, Math::MinFloat), vec3(Math::MaxFloat, Math::MaxFloat, Math::MaxFloat));
        bbox.transform(transform->GetWorldTransfom());
        collider->SetBounds(bbox);
    }

    // Meshes
    std::shared_ptr<CMeshComponent> meshes = GetComponent<CMeshComponent>();
    for (const auto& it : _liquidObject->m_WaterLayers)
    {
        meshes->AddMesh(it);
    }
}

bool Liquid_Instance::Accept(IVisitor & visitor)
{

	return SceneNode3D::Accept(visitor);
}
