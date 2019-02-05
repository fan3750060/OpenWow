#pragma once

class IMesh;
class Texture;
class SceneNodeUI;

class SceneUI : public Object
{
public:
	SceneUI();
	virtual ~SceneUI();

	std::shared_ptr<SceneNodeUI> GetRootNode() const;

	void Render(RenderUIEventArgs& renderEventArgs);

	// Input events
	void OnKeyPressed(KeyEventArgs& e);
	void OnKeyReleased(KeyEventArgs& e);

	void OnMouseMoved(MouseMotionEventArgs& e);
	void OnMouseButtonPressed(MouseButtonEventArgs& e);
	void OnMouseButtonReleased(MouseButtonEventArgs& e);
	void OnMouseWheel(MouseWheelEventArgs& e);

private:
	std::shared_ptr<SceneNodeUI> m_pRootNode;
};
