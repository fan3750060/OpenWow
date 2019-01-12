#include "stdafx.h"

// General
#include "M2_Base_Instance.h"

CM2_Base_Instance::CM2_Base_Instance(std::shared_ptr<M2> _m2Object) :
	m_M2(nullptr),
	m_Attached(nullptr),
	m_Animator(nullptr),
	m_NeedRecalcAnimation(true),
	m_Color(vec4(1.0f, 1.0f, 1.0f, 1.0f)),
	m_Alpha(1.0f)
{
	for (uint8 i = 0; i < SM2_Texture::Type::COUNT; i++)
	{
		m_SpecialTextures[i] = _RenderDevice->GetDefaultTexture();
	}

	if (_m2Object != nullptr)
	{
		setM2(_m2Object);
	}
}

CM2_Base_Instance::~CM2_Base_Instance()
{
	if (m_M2->isAnimated())
	{
		//_Bindings->UnregisterUpdatableObject(this);
	}
}

// CM2_Base_Instance

void CM2_Base_Instance::CreateInstances()
{
	m_M2->CreateInsances(std::static_pointer_cast<CM2_Base_Instance, SceneNode>(shared_from_this()));
}

void CM2_Base_Instance::Attach(const CM2_Part_Attachment* _attachment)
{
	assert1(_attachment != nullptr);
	m_Attached = _attachment;
}
void CM2_Base_Instance::Detach()
{
	m_Attached = nullptr;
}

void CM2_Base_Instance::setM2(std::shared_ptr<M2> _model)
{
	assert1(m_M2 == nullptr);
	assert1(_model != nullptr);
	m_M2 = _model;

	InitAnimator();
	UpdateLocalTransform();
}

// Mesh & textures provider
bool CM2_Base_Instance::isMeshEnabled(uint32 _index) const
{
	return true;
}
void CM2_Base_Instance::setSpecialTexture(SM2_Texture::Type _type, cstring _textureName)
{
	std::shared_ptr<Texture> texture = _RenderDevice->CreateTexture2D(_textureName);
	setSpecialTexture(_type, texture);
}
void CM2_Base_Instance::setSpecialTexture(SM2_Texture::Type _type, std::shared_ptr<Texture> _texture)
{
	if (_texture != nullptr)
	{
		m_SpecialTextures[_type] = _texture;
	}
}
std::shared_ptr<Texture> CM2_Base_Instance::getSpecialTexture(SM2_Texture::Type _type) const
{
	assert1(_type < SM2_Texture::Type::COUNT);
	return m_SpecialTextures[_type];
}



//
//	m_M2->update(_time, _dTime);
//

void CM2_Base_Instance::SetParent(std::weak_ptr<SceneNode> pNode)
{
	SceneNode::SetParent(pNode);

	UpdateLocalTransform();
}

bool CM2_Base_Instance::Accept(IVisitor& visitor)
{
	const BasePass& visitorAsBasePass = reinterpret_cast<BasePass&>(visitor);
	const Camera& camera = *(visitorAsBasePass.GetRenderEventArgs().Camera);

	//float distToCamera2D = (camera.GetTranslation() - getBounds().getCenter()).length() - getBounds().getRadius();
	//if (distToCamera2D > m_QualitySettings.ADT_MCNK_Distance)
	//{
	//	return;
	//}

	// Check frustrum
	if (!checkFrustum(camera))
	{
		return false;
	}

	/*if (m_Attached != nullptr)
	{
		TransRotScaleToLocalTransform();
	}*/

	if (m_M2->isAnimated())
	{
		m_Animator->Update(visitorAsBasePass.GetRenderEventArgs().TotalTime, visitorAsBasePass.GetRenderEventArgs().ElapsedTime);

		//if (m_Object->isBillboard())
		//{
		//m_Object->calc(m_Animator->getAnimID(), m_Animator->getCurrentTime(_time), _time);
		//}
		//else
		//{
		//if (!m_NeedRecalcAnimation)
		//{
		m_M2->calc(m_Animator->getSequenceIndex(), GetWorldTransfom(), m_Animator->getCurrentTime(), static_cast<uint32>(visitorAsBasePass.GetRenderEventArgs().TotalTime));
		//	m_NeedRecalcAnimation = true;
		//}
		//}
	}

	// SceneNode
	return SceneNode::Accept(visitor);
}

void CM2_Base_Instance::InitAnimator()
{
	// Create animator
	if (m_M2->isAnimated())
	{
		m_Animator = std::make_shared<CM2_Animator>(m_M2);
		//_Bindings->RegisterUpdatableObject(this);
	}
}

void CM2_Base_Instance::UpdateLocalTransform()
{
	if (m_Attached != nullptr)
	{
		std::shared_ptr<const CM2_Part_Bone> bone = m_Attached->getBone().lock();
		assert1(bone != nullptr);

		mat4 relMatrix;
		relMatrix = glm::translate(relMatrix, bone->getPivot());

		mat4 absMatrix;
		absMatrix = GetParentWorldTransform() * bone->getTransformMatrix() * relMatrix;
		SetWorldTransform(absMatrix);

		BoundingBox bbox = m_M2->m_Bounds;
		bbox.transform(GetWorldTransfom());
		setBounds(bbox);

		return;
	}

	SceneNode::UpdateLocalTransform();

	BoundingBox bbox = m_M2->m_Bounds;
	bbox.transform(GetWorldTransfom());
	setBounds(bbox);
}
