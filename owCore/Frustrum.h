#pragma once

#include "Plane.h"

class Frustum
{
public:
	const Vec3f& getOrigin() const { return _origin; }
	const Vec3f& getCorner(uint32 index) const { return _corners[index]; }

	void buildViewFrustum(const Matrix4f &transMat, float fov, float aspect, float nearPlane, float farPlane);
	void buildViewFrustum(const Matrix4f &transMat, float left, float right, float bottom, float top, float nearPlane, float farPlane);
	void buildViewFrustum(const Matrix4f &viewMat, const Matrix4f &projMat);
	void buildBoxFrustum(const Matrix4f &transMat, float left, float right, float bottom, float top, float front, float back);

	bool cullSphere(Vec3f pos, float rad) const;
	bool cullBox(BoundingBox& b) const;
	bool cullFrustum(const Frustum& frust) const;

	void calcAABB(Vec3f &mins, Vec3f &maxs) const;

private:
	Plane  _planes[6];  // Planes of frustum
	Vec3f  _origin;
	Vec3f  _corners[8];  // Corner points
};