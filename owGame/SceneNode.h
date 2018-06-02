#pragma once

class SceneNode : public CRefItem
{
public:
	SceneNode();
	SceneNode(SceneNode* _parent);


	cvec3 getTranslate() { return m_Translate; }
	cvec3 getRotate() { return m_Rotate; }
	cvec3 getScale() { return m_Scale; }

	cmat4 getRelTrans() { return m_RelTransform; }
	cmat4 getAbsTrans() { return m_AbsTransform; }

protected:
	void CalculateMatrix();

protected:
	SceneNode*          m_Parent;
	vector<SceneNode*>  m_Childs;

	vec3                m_Translate;
	vec3				m_Rotate;
	vec3                m_Scale;

	mat4				m_RelTransform;
	mat4				m_AbsTransform;

	BoundingBox         m_Bounds;
};