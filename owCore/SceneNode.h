#pragma once

#include "RefItem.h"

class SceneNode : 
	public CRefItem, 
	public ILoadable, 
	public IUpdatable, 
	public IRenderable3D
{
public:
	SceneNode();
	SceneNode(SceneNode* _parent);

	cvec3 getTranslate() const { return m_Translate; }
	cvec3 getRotate() const { return m_Rotate; }
	cquat getRotateQuat() const { return m_RotateQuat; }
	cvec3 getScale() const { return m_Scale; }

	cmat4 getRelTrans() const { return m_RelTransform; }
	cmat4 getAbsTrans() const { return m_AbsTransform; }

	// ILoadable
	virtual bool Load() override { return false; };
	bool IsLoaded() const override { return m_IsLoaded; }
	void SetLoaded() override { m_IsLoaded = true; }

	// IUpdatable
	virtual void Input(double _time, double _dTime) override {};
	virtual void Update(double _time, double _dTime) override {};

	// IRenderable
	virtual void PreRender3D(double _time, double _dTime) override {};
	virtual void Render3D() override {}
	virtual void PostRender3D() override {}
	bool IsVisible() const { return m_IsVisible; }
	void SetVisible(bool _value) override { m_IsVisible = _value; }
	inline void SetDrawOrder(uint32 _order) override { m_DrawOrder = _order; }
	inline uint32 GetDrawOrder() const override { return m_DrawOrder; }

protected:
	void CalculateMatrix(bool _isRotationQuat = false);

protected:
	SceneNode*          m_Parent;
	vector<SceneNode*>  m_Childs;

	vec3                m_Translate;
	vec3				m_Rotate;
	quat				m_RotateQuat;
	vec3                m_Scale;

	mat4				m_RelTransform;
	mat4				m_AbsTransform;

	BoundingBox         m_Bounds;

private: // ILoadable
	bool				m_IsLoaded;

private: // IRenderable
	bool				m_IsVisible;
	uint32				m_DrawOrder;
};