#include "stdafx.h"

// General
#include "M2_Base_Instance.h"

CM2_Base_Instance::CM2_Base_Instance(SceneNode* _parent, M2* _m2Object) :
	SceneNode(_parent),
	m_M2(nullptr),
	m_Attached(nullptr),
	m_Animator(nullptr),
	m_NeedRecalcAnimation(true),
	m_Time(0.0f),
	m_DTime(0.0f),
	m_Color(vec4(1.0f, 1.0f, 1.0f, 1.0f)),
	m_Alpha(1.0f)
{
	for (uint8 i = 0; i < SM2_Texture::Type::COUNT; i++)
	{
		m_SpecialTextures[i] = _Render->TexturesMgr()->DefaultTexture();
	}

	if (_m2Object != nullptr)
	{
		setM2(_m2Object);
	}

	setDrawOrder(21);
}

CM2_Base_Instance::~CM2_Base_Instance()
{
	if (m_M2->isAnimated())
	{
		OW_SAFEDELETE(m_Animator);
		_Bindings->UnregisterUpdatableObject(this);
	}
}

// CM2_Base_Instance

void CM2_Base_Instance::Attach(const CM2_Part_Attachment* _attachment)
{
	assert1(_attachment != nullptr);
	m_Attached = _attachment;
}
void CM2_Base_Instance::Detach()
{
	m_Attached = nullptr;
}

void CM2_Base_Instance::setM2(M2* _model)
{
	assert1(m_M2 == nullptr);
	assert1(_model != nullptr);
	m_M2 = _model;

	InitLocal();
	CalculateMatrix();
}

// Mesh & textures provider
bool CM2_Base_Instance::isMeshEnabled(uint32 _index) const
{
	return true;
}
void CM2_Base_Instance::setSpecialTexture(SM2_Texture::Type _type, cstring _textureName)
{
	R_Texture* texture = GetManager<ITexturesManager>()->Add(_textureName);
	setSpecialTexture(_type, texture);
}
void CM2_Base_Instance::setSpecialTexture(SM2_Texture::Type _type, R_Texture* _texture)
{
	if (_texture != nullptr)
	{
		m_SpecialTextures[_type] = _texture;
	}
}
R_Texture* CM2_Base_Instance::getSpecialTexture(SM2_Texture::Type _type) const
{
	assert1(_type < SM2_Texture::Type::COUNT);
	return m_SpecialTextures[_type];
}



// IUpdatable
void CM2_Base_Instance::Update(double _time, double _dTime)
{
	m_Time = _time;
	m_DTime = _dTime;
	
	m_M2->update(_time, _dTime);
}


// IRenderable3D
bool CM2_Base_Instance::PreRender3D()
{
	if (!checkFrustum())
	{
		return false;
	}

	return true;
}

void CM2_Base_Instance::Render3D()
{
	if (m_Attached != nullptr)
	{
		CalculateMatrix();
	}

	if (m_M2->isAnimated())
	{
		m_Animator->Update(m_Time, m_DTime);

		/*if (m_Object->isBillboard())
		{
		m_Object->calc(m_Animator->getAnimID(), m_Animator->getCurrentTime(_time), _time);
		}
		else
		{*/
		//if (!m_NeedRecalcAnimation)
		//{
		m_M2->calc(m_Animator->getSequenceIndex(), getAbsTrans(), m_Animator->getCurrentTime(), static_cast<uint32>(m_Time));
		//	m_NeedRecalcAnimation = true;
		//}
		//}

		m_M2->Render(this/*getAbsTrans(), m_MeshProvider, m_DoodadColor, m_Animator->getSequenceIndex(), m_Animator->getCurrentTime(), static_cast<uint32>(m_Time)*/);
		return;
	}

	//_Render->DrawBoundingBox(getBounds());

	m_M2->Render(this/*getAbsTrans(), m_MeshProvider, m_DoodadColor, 0, 0, static_cast<uint32>(m_Time)*/);
}

//-----------------
// ISceneNode
//-----------------

void CM2_Base_Instance::InitLocal()
{
	// Create animator
	if (m_M2->isAnimated())
	{
		m_Animator = new CM2_Animator(m_M2);
		_Bindings->RegisterUpdatableObject(this);
	}
}

void CM2_Base_Instance::CalculateMatrix(bool _isRotationQuat)
{
	if (m_Attached != nullptr)
	{
		const CM2_Part_Bone* bone = m_Attached->getBone();
		assert1(bone != nullptr);

		mat4 relMatrix;
		relMatrix.translate(bone->getPivot());

		mat4 absMatrix;
		absMatrix = getParent()->getAbsTrans() * bone->getTransformMatrix() * relMatrix;
		setAbsTrans(absMatrix);

		BoundingBox bbox = m_M2->m_Bounds;
		bbox.transform(getAbsTrans());
		setBounds(bbox);

		return;
	}

	SceneNode::CalculateMatrix(_isRotationQuat);

	BoundingBox bbox = m_M2->m_Bounds;
	bbox.transform(getAbsTrans());
	setBounds(bbox);
}
