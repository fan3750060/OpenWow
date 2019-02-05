#pragma once

#include "Ray.h"
#include "RenderEvents.h"
#include "Viewport.h"

class Camera : public Object
{
public:
	enum class Space
	{
		Local,
		World
	};

	Camera();
	virtual ~Camera() {};

	// TODO: Viewports belong in the rasterizer state.. not the camera. Set view port parameters
	void SetViewport(const Viewport& viewport);
	const Viewport& GetViewport() const;

	const Frustum& GetFrustum() const;

	// Set projection parameters
	void SetProjectionRH(float fovy, float aspect, float zNear, float zFar);
	void SetProjectionLH(float fovy, float aspect, float zNear, float zFar);

	float GetNearClipPlane() const;
	float GetFarClipPlane() const;

	// Set an orthographic projection using screen space coordinates.
	void SetOrthographic(float left, float right, float top, float bottom);

	// Translate

	void TranslateX(float x, Space space = Space::Local);
	void TranslateY(float y, Space space = Space::Local);
	void TranslateZ(float z, Space space = Space::Local);

	void SetTranslate(cvec3 translate);
	vec3 GetTranslation() const;

	// Rotate
	
	void SetEulerAngles(cvec3 eulerAngles);                         // Set Euler angles (in degrees)
	vec3 GetEulerAngles() const;									// Get the current rotation of the camera in Euler angles (degrees)

	void AddPitch(float fPitch, Space space = Space::Local);        // Add this pitch (rotation about the X-axis) in degrees to the current camera's pitch 
	void AddYaw(float fYaw, Space space = Space::Local);            // Add this yaw (rotation about the Y-axis) in degrees to the current camera's yaw
	void AddRoll(float fRoll, Space space = Space::Local);          // Add this roll (rotation about the Z-axis) in degrees to the current camera's roll

	void AddRotation(const glm::quat& rot);                         // Add rotation to the current rotation (always in local space)

	void SetRotate(float pitch, float yaw, float roll);
	void SetRotate(cvec3 rotate);
	void SetRotate(const glm::quat& rot);
	glm::quat GetRotation() const;

	// Pivot
	
	void SetPivotDistance(float pivotDistance);                     // If the pivot distance > 0 then the camera will rotate around a pivot point that is pivotDistance along the lookAt vector of the camera.
	float GetPivotDistance() const;
	vec3 GetPivotPoint() const;                                     // Get the camera's pivot point in world space

	bool IsDirty() const;
	
	// Matrices

	void SetViewMatrix(cmat4 viewMatrix);                           // Directly set the view matrix
	mat4 GetViewMatrix() const;

	void SetProjectionMatrix(cmat4 projectionMatrix);               // Directly set the projection matrix
	mat4 GetProjectionMatrix() const;

	// Get the view projection inverse matrix (useful for picking)
	mat4 GetViewProjectionInverseMatrix() const;

	// Converts a screen point to a ray in world space.
	Ray	ScreenPointToRay(cvec2 screenPoint) const;

	// For arcball camera, call this function with client-space coordinates when the mouse is clicked on the screen.
	void OnMousePressed(MouseButtonEventArgs& e);
	void OnMouseMoved(MouseMotionEventArgs& e);

protected:
	vec3 ProjectOntoUnitSphere(glm::ivec2 screenPos);

	virtual void UpdateViewMatrix();
	virtual void UpdateViewProjectionInverse();

	// View port parameters
	Viewport    m_Viewport;

	// Frustum
	Frustum     m_Frustum;

	// Projection parameters
	float       m_fVFOV;
	float       m_fAspect;
	float       m_fNear;
	float       m_fFar;

	// World space parameters
	vec3        m_Translate;
	glm::quat   m_Rotate;

	// Used for arcball camera
	glm::vec3   m_PreviousPoint;
	float       m_PivotDistance;

	// View matrix
	mat4        m_ViewMatrix;
	// Projection matrix.
	mat4        m_ProjectionMatrix;

	mat4	    m_ViewProjectionInverse;

	// True if the view matrix needs to be updated.
	bool        m_bViewDirty;
	// True if the view projection inverse matrix needs to be updated.
	bool		m_bViewProjectionInverseDirty;
};