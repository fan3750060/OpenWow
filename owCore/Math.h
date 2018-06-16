#pragma once

#include "Vec2f.h"
#include "Vec3f.h"
#include "Vec4f.h"
#include "Quaternion.h"
#include "Matrix.h"

// -------------------------------------------------------------------------------------------------
// Intersection
// -------------------------------------------------------------------------------------------------

inline bool rayTriangleIntersection
(
	cvec3 rayOrig, 
	cvec3 rayDir,
	cvec3 vert0, 
	cvec3 vert1, 
	cvec3 vert2,
	Vec3f &intsPoint
)
{
	// Idea: Tomas Moeller and Ben Trumbore
	// in Fast, Minimum getRenderStorage Ray/Triangle Intersection 

	// Find vectors for two edges sharing vert0
	Vec3f edge1 = vert1 - vert0;
	Vec3f edge2 = vert2 - vert0;

	// Begin calculating determinant - also used to calculate U parameter
	Vec3f pvec = rayDir.cross(edge2);

	// If determinant is near zero, ray lies in plane of triangle
	float det = edge1.dot(pvec);


	// *** Culling branch ***
	/*if( det < Math::Epsilon )return false;

	// Calculate distance from vert0 to ray origin
	Vec3f tvec = rayOrig - vert0;

	// Calculate U parameter and test bounds
	float u = tvec.dot( pvec );
	if (u < 0 || u > det ) return false;

	// Prepare to test V parameter
	Vec3f qvec = tvec.cross( edge1 );

	// Calculate V parameter and test bounds
	float v = rayDir.dot( qvec );
	if (v < 0 || u + v > det ) return false;

	// Calculate t, scale parameters, ray intersects triangle
	float t = edge2.dot( qvec ) / det;*/


	// *** Non-culling branch ***
	if (det > -Math::Epsilon && det < Math::Epsilon) return 0;
	float inv_det = 1.0f / det;

	// Calculate distance from vert0 to ray origin
	Vec3f tvec = rayOrig - vert0;

	// Calculate U parameter and test bounds
	float u = tvec.dot(pvec) * inv_det;
	if (u < 0.0f || u > 1.0f) return 0;

	// Prepare to test V parameter
	Vec3f qvec = tvec.cross(edge1);

	// Calculate V parameter and test bounds
	float v = rayDir.dot(qvec) * inv_det;
	if (v < 0.0f || u + v > 1.0f) return 0;

	// Calculate t, ray intersects triangle
	float t = edge2.dot(qvec) * inv_det;


	// Calculate intersection point and test ray length and direction
	intsPoint = rayOrig + rayDir * t;
	Vec3f vec = intsPoint - rayOrig;
	if (vec.dot(rayDir) < 0 || vec.length() > rayDir.length()) return false;

	return true;
}


inline bool rayAABBIntersection
(
	cvec3 rayOrig, 
	cvec3 rayDir,				
	cvec3 mins, 
	cvec3 maxs
)
{
	// SLAB based optimized ray/AABB intersection routine
	// Idea taken from http://ompf.org/ray/

	float l1 = (mins.x - rayOrig.x) / rayDir.x;
	float l2 = (maxs.x - rayOrig.x) / rayDir.x;
	float lmin = minf(l1, l2);
	float lmax = maxf(l1, l2);

	l1 = (mins.y - rayOrig.y) / rayDir.y;
	l2 = (maxs.y - rayOrig.y) / rayDir.y;
	lmin = maxf(minf(l1, l2), lmin);
	lmax = minf(maxf(l1, l2), lmax);

	l1 = (mins.z - rayOrig.z) / rayDir.z;
	l2 = (maxs.z - rayOrig.z) / rayDir.z;
	lmin = maxf(minf(l1, l2), lmin);
	lmax = minf(maxf(l1, l2), lmax);

	if ((lmax >= 0.0f) & (lmax >= lmin))
	{
		// Consider length
		const Vec3f rayDest = rayOrig + rayDir;
		Vec3f rayMins(minf(rayDest.x, rayOrig.x), minf(rayDest.y, rayOrig.y), minf(rayDest.z, rayOrig.z));
		Vec3f rayMaxs(maxf(rayDest.x, rayOrig.x), maxf(rayDest.y, rayOrig.y), maxf(rayDest.z, rayOrig.z));
		return
			(rayMins.x < maxs.x) && (rayMaxs.x > mins.x) &&
			(rayMins.y < maxs.y) && (rayMaxs.y > mins.y) &&
			(rayMins.z < maxs.z) && (rayMaxs.z > mins.z);
	}
	else
	{
		return false;
	}
}


inline float nearestDistToAABB
(
	cvec3 pos, 
	cvec3 mins, 
	cvec3 maxs
)
{
	const Vec3f center = (mins + maxs) * 0.5f;
	const Vec3f extent = (maxs - mins) * 0.5f;

	Vec3f nearestVec;
	nearestVec.x = maxf(0, fabsf(pos.x - center.x) - extent.x);
	nearestVec.y = maxf(0, fabsf(pos.y - center.y) - extent.y);
	nearestVec.z = maxf(0, fabsf(pos.z - center.z) - extent.z);

	return nearestVec.length();
}

inline vec3 screenToWord
(
	cvec2 _mousePos, 
	cvec2 windowSize, 
	cmat4 projection_matrix, 
	cmat4 view_matrix
)
{
	float x = (2.0f * _mousePos.x) / windowSize.x - 1.0f;
	float y = 1.0f - (2.0f * _mousePos.y) / windowSize.y;

	vec2 ray_nds = vec2(x, y);

	vec4 ray_clip = vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);

	vec4 ray_eye = projection_matrix.inverted() * ray_clip;
	ray_eye = vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

	vec3 ray_wor = view_matrix.inverted() * ray_eye;
	ray_wor = ray_wor.normalized();

	return ray_wor;
}
