#pragma once

class CCameraController
{
public:
	CCameraController(std::shared_ptr<Camera> _camera = nullptr);
	~CCameraController();

	void Init(std::shared_ptr<RenderWindow> _renderWindow);

	std::shared_ptr<Camera> GetCamera() const;

	// Input
	void OnUpdate(UpdateEventArgs& e);
	void OnKeyPressed(KeyEventArgs& e);
	void OnKeyReleased(KeyEventArgs& e);
	void OnMouseButtonPressed(MouseButtonEventArgs& e);
	void OnMouseButtonReleased(MouseButtonEventArgs& e);
	void OnMouseMoved(MouseMotionEventArgs& e);
	void OnMouseWheel(MouseWheelEventArgs& e);

private:
	std::shared_ptr<Camera>  m_Camera;
	vec2                     m_PreviousMousePosition;

	// Translation movement
	float Forward, Back, Left, Right, Up, Down;

	// Rotation movement
	float RollCW, RollCCW;
	float Pitch, Yaw;

	// Move in/out from pivot point.
	float PivotTranslate;

	// Do you want to go faster?
	bool TranslateFaster;
	bool RotateFaster;
};