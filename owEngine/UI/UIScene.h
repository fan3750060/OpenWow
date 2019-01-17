#pragma once

class IMesh;
class UINode;
class RenderEventArgs;
class IVisitor;

class UIScene : public Object
{
public:
	virtual std::shared_ptr<UINode> GetRootNode() const = 0;

	virtual void Accept(IVisitor& visitor) = 0;

	/**
	 * Input events
	 */
	virtual void OnKeyPressed(KeyEventArgs& e) = 0;
	virtual void OnKeyReleased(KeyEventArgs& e) = 0;

	virtual void OnMouseMoved(MouseMotionEventArgs& e) = 0;
	virtual void OnMouseButtonPressed(MouseButtonEventArgs& e) = 0;
	virtual void OnMouseButtonReleased(MouseButtonEventArgs& e) = 0;
	virtual void OnMouseWheel(MouseWheelEventArgs& e) = 0;
};