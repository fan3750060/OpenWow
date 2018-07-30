#pragma once

#include "M2.h"

class CM2_Base_Instance : public SceneNode
{
public:
	CM2_Base_Instance(SceneNode* _parent, SmartM2Ptr _m2Object = nullptr);
	virtual ~CM2_Base_Instance();

	// CM2_Base_Instance
	void setM2(SmartM2Ptr _model);
	SmartM2Ptr getM2() const { return m_M2; }

	void Attach(const CM2_Part_Attachment* _attachment);
	void Detach();

	// Color & Alpha
	void setColor(vec4 _color) { m_Color = _color; }
	cvec4 getColor() const { return m_Color; }
	void setAlpha(float _alpha) { m_Alpha = _alpha; }
	float getAlpha() const { return m_Alpha; }

	// Mesh & textures provider
	virtual bool isMeshEnabled(uint32 _index) const;
	void setSpecialTexture(SM2_Texture::Type _type, cstring _textureName);
	void setSpecialTexture(SM2_Texture::Type _type, SharedTexturePtr _texture);
	SharedTexturePtr getSpecialTexture(SM2_Texture::Type _type) const;

	// Animations
	std::shared_ptr<CM2_Animator> getAnimator() { return m_Animator; }
	double m_Time;  // TODO: Delete me!!!
	double m_DTime; // TODO: Delete me!!!

	// IUpdatable
	void Update(double _time, double _dTime) override;

	// IRenderable3D
	bool PreRender3D() override;
	void Render3D() override;

protected:
	void InitLocal();
	virtual void CalculateMatrix(bool _isRotationQuat = false) override;

private:
	// Color & Alpha
	vec4				m_Color;
	float				m_Alpha;

	// Mesh & Replaces textures
	SharedTexturePtr m_SpecialTextures[SM2_Texture::Type::COUNT];

	// Animtion
	std::shared_ptr<CM2_Animator>		m_Animator;
	bool				m_NeedRecalcAnimation;

private: // PARENT
	SmartM2Ptr					m_M2;
	const CM2_Part_Attachment*	m_Attached;
};