#include "stdafx.h"

// General
#include "Camera.h"

// Additional
#include "Render.h"

Camera::Camera(vec3 position, vec3 up, float roll, float pitch) :
	Direction(vec3(0.0f, 0.0f, -1.0f)),
	MovementSpeed(SPEED),
	MouseSensitivity(SENSITIVTY),
	m_UseDir(false),
	m_DisableUpdate(false),
	m_Engine(GetManager<IEngine>()),
	m_VideoSettings(GetSettingsGroup<CGroupVideo>())
{

	Position = position;
	//WorldUp = up;
	Roll = roll;
	Pitch = pitch;

	enableFreeCamera = false;
	cameraSprint = false;
	cameraSlow = false;

	_Bindings->RegisterUpdatableObject(this);
	_Bindings->RegisterInputListener(this, 100);
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float roll, float pitch) :
	Direction(vec3(0.0f, 0.0f, -1.0f)),
	MovementSpeed(SPEED),
	MouseSensitivity(SENSITIVTY),
	m_UseDir(false),
	m_DisableUpdate(false),
	m_Engine(GetManager<IEngine>()),
	m_VideoSettings(GetSettingsGroup<CGroupVideo>())
{

	Position = vec3(posX, posY, posZ);

	Roll = roll;
	Pitch = pitch;

	enableFreeCamera = false;
	cameraSprint = false;
	cameraSlow = false;

	_Bindings->RegisterUpdatableObject(this);
	_Bindings->RegisterInputListener(this, 100);
}

Camera::~Camera()
{
	_Bindings->UnregisterInputListener(this);
	_Bindings->UnregisterUpdatableObject(this);
}

void Camera::Input(CInput* _input, double _time, double _dTime)
{
	float speed = 5.0f;

	if (cameraSlow)		speed *= 0.1f;
	if (cameraSprint)	speed *= 2.0f;

	if (_input->IsKeyPressed(OW_KEY_W))	ProcessKeyboard(FORWARD, speed);
	if (_input->IsKeyPressed(OW_KEY_S))	ProcessKeyboard(BACKWARD, speed);
	if (_input->IsKeyPressed(OW_KEY_A))	ProcessKeyboard(LEFT, speed);
	if (_input->IsKeyPressed(OW_KEY_D))	ProcessKeyboard(RIGHT, speed);
}

void Camera::Update(double _time, double _dTime)
{
	if (!m_NeedUpdate || m_DisableUpdate)
	{
		return;
	}
	m_NeedUpdate = false;

	if (!m_UseDir)
	{
		// Calculate the new Front vector
		vec3 front;
		/*front.x = -(sinf(degToRad(Roll)) * cos(degToRad(Pitch)));
		front.y = sin(degToRad(Pitch));
		front.z = -(cosf(degToRad(Roll)) * cos(degToRad(Pitch)));*/

		front.x = cos(glm::radians(Roll)) * cos(glm::radians(Pitch)); // y
		front.y = sin(glm::radians(Pitch));                       // z
		front.z = sin(glm::radians(Roll)) * cos(glm::radians(Pitch)); // x

		Direction = glm::normalize(front);
	}

	Direction = glm::normalize(Direction);

	// Also re-calculate the Right and Up vector
	CameraRight = glm::normalize(glm::cross(Direction,vec3(0.0f, 1.0f, 0.0f)));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	CameraUp = glm::cross(CameraRight, Direction);

	_viewMat = glm::lookAt(Position, Position + Direction, CameraUp);

	// View matrix
	//setTransform(Position, vec3(Pitch, Roll, 0.0), vec3(1.0f, 1.0f, 1.0f));

	onPostUpdate();
}

void Camera::OnMouseMoved(cvec2 _mousePos)
{
	if (enableFreeCamera)
	{
		vec2 mouseDelta = (_mousePos - lastMousePos) / m_VideoSettings.GetWindowSize();
		ProcessMouseMovement(mouseDelta.x, -mouseDelta.y);
		_Render->getAdapter()->setMousePosition(lastMousePos);
	}
}

bool Camera::OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos)
{
	if (_button == OW_MOUSE_BUTTON_LEFT)
	{
		enableFreeCamera = true;
		lastMousePos = _mousePos;
		_Render->getAdapter()->HideCursor();
		return true;
	}

	return false;
}

bool Camera::OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos)
{
	enableFreeCamera = false;
	lastMousePos = vec2();
	_Render->getAdapter()->ShowCursor();
	return true;
}

bool Camera::OnKeyboardPressed(int _key, int _scancode, int _mods)
{
	if (_key == OW_KEY_X)
	{
		cameraSprint = true;
		return true;
	}

	if (_key == OW_KEY_Z)
	{
		cameraSlow = true;
		return true;
	}

	return false;
}

bool Camera::OnKeyboardReleased(int _key, int _scancode, int _mods)
{
	if (_key == OW_KEY_X)
	{
		cameraSprint = false;
		return true;
	}

	if (_key == OW_KEY_Z)
	{
		cameraSlow = false;
		return true;
	}

	return false;
}


void Camera::Render()
{
	_Render->DrawSphere(mat4(), Position, 10, vec4(1.0f, 0.0f, 0.0f, 1.0f));
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
	{
		Position += Direction * velocity;
	}
	if (direction == BACKWARD)
	{
		Position -= Direction * velocity;
	}
	if (direction == LEFT)
	{
		Position -= CameraRight * velocity;
	}
	if (direction == RIGHT)
	{
		Position += CameraRight * velocity;
	}

	m_NeedUpdate = true;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Roll += xoffset;
	Pitch += yoffset;

	if (constrainPitch)
	{
		if (Pitch > 89.0f)
		{
			Pitch = 89.0f;
		}

		if (Pitch < -89.0f)
		{
			Pitch = -89.0f;
		}
	}

	m_NeedUpdate = true;
}

//

void Camera::setupViewportSize(float x, float y, float w, float h)
{
	_vpX = x;
	_vpY = y;
	_vpWidth = w;
	_vpHeight = h;
}

void Camera::setupViewParams(float fov, float aspect, float nearPlane, float farPlane)
{
	_fov = fov;
	tan = tanf(fov / 2.0f);
	asp = aspect;

	float ymax = nearPlane * tan;
	float xmax = ymax * aspect;

	_frustLeft = -xmax;
	_frustRight = xmax;

	_frustBottom = -ymax;
	_frustTop = ymax;

	_frustNear = nearPlane;
	_frustFar = farPlane;

	// setting view params implicitly disables the manual projection matrix 
	_manualProjMat = false;
	_orthographic = false;
}

void Camera::setProjectionMatrix(float* projMat)
{
	memcpy(glm::value_ptr(_projMat), projMat, 16 * sizeof(float));
	_manualProjMat = true;
}

void Camera::onPostUpdate()
{
	// Calculate projection matrix if not using a manually set one
	if (!_manualProjMat)
	{
		if (!_orthographic)
		{
			_projMat = glm::perspective(_fov, asp, _frustNear, _frustFar);
		}
		else
		{
			_projMat = glm::ortho(_frustLeft, _frustRight, _frustBottom, _frustTop);
		}
	}

	// Update frustum
	_frustum.buildViewFrustum(_viewMat, _projMat);
}

void Camera::CreateRenderable()
{
	float nh = _frustNear * tan;
	float nw = nh * asp;

	float fh = _frustFar * tan;
	float fw = fh * asp;

	vec3 nc (_frustNear);
	vec3 fc (_frustFar);

	// compute the 4 corners of the frustum on the near plane
	vec3 ntl = nc + CameraUp * nh - CameraRight * nw;
	vec3 ntr = nc + CameraUp * nh + CameraRight * nw;
	vec3 nbl = nc - CameraUp * nh - CameraRight * nw;
	vec3 nbr = nc - CameraUp * nh + CameraRight * nw;

	// compute the 4 corners of the frustum on the far plane
	vec3 ftl = fc + CameraUp * fh - CameraRight * fw;
	vec3 ftr = fc + CameraUp * fh + CameraRight * fw;
	vec3 fbl = fc - CameraUp * fh - CameraRight * fw;
	vec3 fbr = fc - CameraUp * fh + CameraRight * fw;

	vector<vec3> verts;

	// Near
	verts.push_back(ntl);
	verts.push_back(ntr);
	verts.push_back(nbl);//
	verts.push_back(nbl);
	verts.push_back(ntr);
	verts.push_back(nbr);

	// Lines
	verts.push_back(ntl);
	verts.push_back(ftl);
	verts.push_back(nbl);
	verts.push_back(nbl);
	verts.push_back(ftl);
	verts.push_back(fbl);

	verts.push_back(ntr);
	verts.push_back(ftr);
	verts.push_back(nbr);
	verts.push_back(nbr);
	verts.push_back(ftr);
	verts.push_back(fbr);


	// Near
	verts.push_back(ftl);
	verts.push_back(ftr);
	verts.push_back(fbl);//
	verts.push_back(fbl);
	verts.push_back(ftr);
	verts.push_back(fbr);


	// Vertex buffer
	SharedBufferPtr __vb = _Render->r.createVertexBuffer(static_cast<uint32>(verts.size()) * sizeof(vec3), verts.data());
	__geom = _Render->r.beginCreatingGeometry(PRIM_TRILIST, _Render->getRenderStorage()->__layout_GxVBF_P);
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 0, 12);
	__geom->finishCreatingGeometry();
}
