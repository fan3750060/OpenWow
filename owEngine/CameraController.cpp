#include "stdafx.h"

// General
#include "CameraController.h"

CCameraController::CCameraController(std::shared_ptr<Camera> _camera)
	: m_Camera(_camera)
	, Forward(0.0f)
	, Back(0.0f)
	, Left(0.0f)
	, Right(0.0f)
	, Up(0.0f)
	, Down(0.0f)
	, RollCW(0.0f)
	, RollCCW(0.0f)
	, Pitch(0.0f)
	, Yaw(0.0f)
	, PivotTranslate(0.0f)
	, TranslateFaster(false)
	, RotateFaster(false)
{
	if (m_Camera == nullptr)
		m_Camera = std::make_shared<Camera>();
}

CCameraController::~CCameraController()
{
}

void CCameraController::Init(std::shared_ptr<RenderWindow> _renderWindow)
{
	_renderWindow->Update               += boost::bind(&CCameraController::OnUpdate, this, _1);
	_renderWindow->MouseButtonPressed   += boost::bind(&CCameraController::OnMouseButtonPressed, this, _1);
	_renderWindow->MouseButtonReleased  += boost::bind(&CCameraController::OnMouseButtonReleased, this, _1);
	_renderWindow->MouseMoved           += boost::bind(&CCameraController::OnMouseMoved, this, _1);
	_renderWindow->MouseWheel           += boost::bind(&CCameraController::OnMouseWheel, this, _1);
	_renderWindow->KeyPressed           += boost::bind(&CCameraController::OnKeyPressed, this, _1);
	_renderWindow->KeyReleased          += boost::bind(&CCameraController::OnKeyReleased, this, _1);
}

std::shared_ptr<Camera> CCameraController::GetCamera() const
{
	return m_Camera;
}

std::shared_ptr<const Camera> CCameraController::GetCameraConst() const
{
	return m_Camera;
}

void CCameraController::OnUpdate(UpdateEventArgs& e)
{
	float moveMultiplier = (TranslateFaster) ? 100 : 50;
	//float rotateMultiplier = (RotateFaster) ? 600 : 300;

	m_Camera->TranslateX((Right - Left) * e.ElapsedTime * moveMultiplier);
	m_Camera->TranslateY((Up - Down) * e.ElapsedTime * moveMultiplier);
	m_Camera->TranslateZ((Back - Forward) * e.ElapsedTime * moveMultiplier);
	//m_Camera->AddPitch(Pitch * 60.0f * e.ElapsedTime * rotateMultiplier, Camera::Space::Local);
	//m_Camera->AddYaw(Yaw * 60.0f * e.ElapsedTime * rotateMultiplier, Camera::Space::World);
}

void CCameraController::OnKeyPressed(KeyEventArgs& e)
{
	CGroupQuality& qualitySettings = GetSettingsGroup<CGroupQuality>();

	switch (e.Key)
	{
	case KeyCode::W:
	{
		Forward = 1.0f;
	}
	break;
	case KeyCode::A:
	{
		Left = 1.0f;
	}
	break;
	case KeyCode::S:
	{
		Back = 1.0f;
	}
	break;
	case KeyCode::D:
	{
		Right = 1.0f;
	}
	break;
	case KeyCode::Q:
	{
		Down = 1.0f;
	}
	break;
	case KeyCode::E:
	{
		Up = 1.0f;
	}
	break;
	case KeyCode::ShiftKey:
	{
		TranslateFaster = true;
		RotateFaster = true;
	}
	break;
	case KeyCode::D0:
	{
		qualitySettings.draw_mcnk = !qualitySettings.draw_mcnk;
	}
	break;
	}

}

void CCameraController::OnKeyReleased(KeyEventArgs& e)
{
	switch (e.Key)
	{
	case KeyCode::W:
	{
		Forward = 0.0f;
	}
	break;
	case KeyCode::A:
	{
		Left = 0.0f;
	}
	break;
	case KeyCode::S:
	{
		Back = 0.0f;
	}
	break;
	case KeyCode::D:
	{
		Right = 0.0f;
	}
	break;
	case KeyCode::Q:
	{
		Down = 0.0f;
	}
	break;
	case KeyCode::E:
	{
		Up = 0.0f;
	}
	break;
	case KeyCode::R:
	{
		RollCW = 0.0f;
	}
	break;
	case KeyCode::F:
	{
		RollCCW = 0.0f;
	}
	break;
	case KeyCode::ShiftKey:
	{
		TranslateFaster = false;
		RotateFaster = false;
	}
	break;
	}
}

void CCameraController::OnMouseButtonPressed(MouseButtonEventArgs& e)
{
	m_Camera->OnMousePressed(e);

	m_PreviousMousePosition = glm::vec2(e.X, e.Y);
}

void CCameraController::OnMouseButtonReleased(MouseButtonEventArgs& e)
{
	//m_Camera->OnMouseReleased(e);

	glm::vec2 currentMousePosition = glm::vec2(e.X, e.Y);
	float offset = glm::distance(m_PreviousMousePosition, currentMousePosition);
}

void CCameraController::OnMouseMoved(MouseMotionEventArgs& e)
{
	if (e.LeftButton)
	{
		m_Camera->OnMouseMoved(e);
	}
}

void CCameraController::OnMouseWheel(MouseWheelEventArgs& e)
{
	//float fPivot = m_Camera->GetPivotDistance();
	//fPivot -= e.WheelDelta * (TranslateFaster ? 1.0f : 0.1f);
	//m_Camera->SetPivotDistance(fPivot);
}