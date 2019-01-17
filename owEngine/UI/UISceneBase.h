#pragma once

#include "UIScene.h"

class IMesh;
class Texture;
class UINode;

// A model base class.
// Implements a basic model loader using Assimp.
class UISceneBase : public UIScene
{
public:
	UISceneBase();
	virtual ~UISceneBase();

	virtual std::shared_ptr<UINode> GetRootNode() const;

	virtual void Accept(IVisitor& visitor);

	/**
	 * Input events
	 */
	void OnKeyPressed(KeyEventArgs& e);
	void OnKeyReleased(KeyEventArgs& e);

	void OnMouseMoved(MouseMotionEventArgs& e);
	void OnMouseButtonPressed(MouseButtonEventArgs& e);
	void OnMouseButtonReleased(MouseButtonEventArgs& e);
	void OnMouseWheel(MouseWheelEventArgs& e);

private:
	std::shared_ptr<UINode> m_pRootNode;
};
