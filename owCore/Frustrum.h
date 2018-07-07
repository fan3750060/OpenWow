#pragma once

#include "Plane.h"

class Frustum
{
public:

	Frustum& operator=(const Frustum& _other)
	{
		m_PlanesCount = _other.m_PlanesCount;
		for (uint32 i = 0; i < this->m_PlanesCount; i++)
		{
			m_Planes[i] = _other.m_Planes[i];
		}
	}

	const Plane* getPlanes() const { return &m_Planes[0]; }
	const uint32 getPlanesCnt() const { return m_PlanesCount; }
	const Vec3f& getOrigin() const { return m_Origin; }
	const Vec3f& getCorner(uint32 index) const { return m_Corners[index]; }

	void buildCustomFrustrum(const Plane* _planes, uint32 _planesCount);
	void buildViewFrustum(const Matrix4f &transMat, float fov, float aspect, float nearPlane, float farPlane);
	void buildViewFrustum(const Matrix4f &transMat, float left, float right, float bottom, float top, float nearPlane, float farPlane);
	void buildViewFrustum(const Matrix4f &viewMat, const Matrix4f &projMat);
	void buildBoxFrustum(const Matrix4f &transMat, float left, float right, float bottom, float top, float front, float back);

	bool cullSphere(Vec3f pos, float rad) const;
	bool cullBox(const BoundingBox& b) const;
	bool cullFrustum(const Frustum& frust) const;
	bool cullPoly(const vec3* verts, uint32 nums) const;

	void calcAABB(Vec3f &mins, Vec3f &maxs) const;



private:
	Plane  m_Planes[20];  // Planes of frustum
	uint8  m_PlanesCount;

	Vec3f  m_Origin;
	Vec3f  m_Corners[8];  // Corner points
};