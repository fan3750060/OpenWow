#pragma once

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 1.0f;
const float SENSITIVTY = 20.1f;

class Camera : public IUpdatable, public IInputListener
{
public:
	Camera(vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
	virtual ~Camera();

	void EnableUpdate() { m_DisableUpdate = false; }
	void DisableUpdate() { m_DisableUpdate = true; }

	// IUpdatable
	void Input(CInput* _input, double _time, double _dTime) override;
	void Update(double _time, double _dTime) override;

	// IInputListener
	void OnMouseMoved(cvec2 _mousePos) override;
	bool OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos) override;
	bool OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos) override;
	bool OnMouseWheel(int _yoffset) override { return false; }
	bool OnKeyboardPressed(int _key, int _scancode, int _mods) override;
	bool OnKeyboardReleased(int _key, int _scancode, int _mods) override;
	bool OnCharInput(uint32 _char) override { return false; }

	void Render();

	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

	void setupViewportSize(float x, float y, float w, float h);
	void setupViewParams(float fov, float aspect, float nearPlane, float farPlane);
	void setProjectionMatrix(float* projMat);
	void setViewMatrix(cmat4 _mat) { _viewMat = _mat; onPostUpdate(); }
	void onPostUpdate();

	const Frustum& getFrustum() const { return _frustum; }
	cmat4 getViewMat() const { return _viewMat; }
	cmat4 getProjMat() const { return _projMat; }

	void SetNeedUpdate() { m_NeedUpdate = true; }

	void CreateRenderable();

public:
	// Camera Attributes
	vec3 Position;
	vec3 Direction;

	vec3 CameraUp;
	vec3 CameraRight;

	// Eular Angles
	float Roll;
	float Pitch;

	// Sets
	bool m_NeedUpdate;
	bool m_UseDir;

	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	bool enableFreeCamera;
	bool cameraSprint;
	bool cameraSlow;
	vec2 lastMousePos;
	bool m_DisableUpdate;

	IEngine*		m_Engine;
	CGroupVideo&	m_VideoSettings;

public:
	float               _fov;
	float				tan;
	float				asp;
	mat4               _viewMat, _projMat;
	Frustum             _frustum;
	int					_vpX, _vpY, _vpWidth, _vpHeight;

	float               _frustLeft, _frustRight, _frustBottom, _frustTop;
	float               _frustNear, _frustFar;

	bool                _orthographic;  // Perspective or orthographic frustum?
	bool                _manualProjMat; // Projection matrix manually set?
	std::shared_ptr<Mesh>		__geom;
};