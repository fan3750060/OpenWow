#pragma once

// Forward BEGIN
class Camera;
// Forward END

struct ICameraController
{
	virtual void Init(std::shared_ptr<RenderWindow> _renderWindow) = 0;

	virtual std::shared_ptr<Camera> GetCamera() const = 0;
};
