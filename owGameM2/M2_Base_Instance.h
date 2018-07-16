#pragma once

#include "M2.h"

class CM2_Base_Instance : public SceneNode
{
public:
	CM2_Base_Instance(SceneNode* _parent, M2* _m2Object = nullptr);
	virtual ~CM2_Base_Instance();

	// CM2_Base_Instance
	void InitLocal();
	void setM2(M2* _model) { assert1(m_M2 == nullptr); m_M2 = _model; CalculateMatrix(); }
	M2* getM2() const { return m_M2; }

	// Color & Alpha
	void setColor(vec4 _color) { m_Color = _color; }
	cvec4 getColor() const { return m_Color; }
	void setAlpha(float _alpha) { m_Alpha = _alpha; }
	float getAlpha() const { return m_Alpha; }

	// Mesh provider
	virtual bool isMeshEnabled(uint32 _index) const { return true; }
	void setSpecialTexture(SM2_Texture::Type _type, cstring _textureName);
	void setSpecialTexture(SM2_Texture::Type _type, R_Texture* _texture);
	R_Texture* getSpecialTexture(SM2_Texture::Type _type) const;

	// Animations
	CM2_Animator* getAnimator() { return m_Animator; }
	double m_Time;  // TODO: Delete me!!!
	double m_DTime; // TODO: Delete me!!!

	// ISceneNode
	virtual void CalculateMatrix(bool _isRotationQuat = false) override;

	// IUpdatable
	void Update(double _time, double _dTime) override;

	// IRenderable3D
	bool PreRender3D() override;
	void Render3D() override;

private:
	// Color & Alpha
	vec4				m_Color;
	float				m_Alpha;

	// Mesh & Replaces textures
	R_Texture* m_SpecialTextures[SM2_Texture::Type::COUNT];

	// Animtion
	CM2_Animator*		m_Animator;
	bool				m_NeedRecalcAnimation;

private: // PARENT
	SmartM2Ptr			m_M2;
};