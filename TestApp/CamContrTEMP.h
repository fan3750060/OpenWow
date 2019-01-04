#pragma once


Camera g_Camera;
glm::vec2 g_PreviousMousePosition;

struct CameraMovement
{
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

	CameraMovement()
		: Forward(0.0f)
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
	{}
};
CameraMovement g_CameraMovement;

void OnUpdate(UpdateEventArgs& e)
{
	float moveMultiplier = (g_CameraMovement.TranslateFaster) ? 600 : 300;
	//float rotateMultiplier = (g_CameraMovement.RotateFaster) ? 600 : 300;

	g_Camera.TranslateX((g_CameraMovement.Right - g_CameraMovement.Left) * e.ElapsedTime * moveMultiplier);
	g_Camera.TranslateY((g_CameraMovement.Up - g_CameraMovement.Down) * e.ElapsedTime * moveMultiplier);
	g_Camera.TranslateZ((g_CameraMovement.Back - g_CameraMovement.Forward) * e.ElapsedTime * moveMultiplier);
	//g_Camera.AddPitch(g_CameraMovement.Pitch * 60.0f * e.ElapsedTime * rotateMultiplier, Camera::Space::Local);
	//g_Camera.AddYaw(g_CameraMovement.Yaw * 60.0f * e.ElapsedTime * rotateMultiplier, Camera::Space::World);
}

void OnKeyPressed(KeyEventArgs& e)
{
	CGroupQuality& qualitySettings = GetSettingsGroup<CGroupQuality>();

	switch (e.Key)
	{
	case KeyCode::W:
	{
		g_CameraMovement.Forward = 1.0f;
	}
	break;
	case KeyCode::A:
	{
		g_CameraMovement.Left = 1.0f;
	}
	break;
	case KeyCode::S:
	{
		g_CameraMovement.Back = 1.0f;
	}
	break;
	case KeyCode::D:
	{
		g_CameraMovement.Right = 1.0f;
	}
	break;
	case KeyCode::Q:
	{
		g_CameraMovement.Down = 1.0f;
	}
	break;
	case KeyCode::E:
	{
		g_CameraMovement.Up = 1.0f;
	}
	break;
	case KeyCode::ShiftKey:
	{
		g_CameraMovement.TranslateFaster = true;
		g_CameraMovement.RotateFaster = true;
	}
	break;
	case KeyCode::D0:
	{
		qualitySettings.draw_mcnk = !qualitySettings.draw_mcnk;
	}
	break;
	}

}

void OnKeyReleased(KeyEventArgs& e)
{
	switch (e.Key)
	{
	case KeyCode::W:
	{
		g_CameraMovement.Forward = 0.0f;
	}
	break;
	case KeyCode::A:
	{
		g_CameraMovement.Left = 0.0f;
	}
	break;
	case KeyCode::S:
	{
		g_CameraMovement.Back = 0.0f;
	}
	break;
	case KeyCode::D:
	{
		g_CameraMovement.Right = 0.0f;
	}
	break;
	case KeyCode::Q:
	{
		g_CameraMovement.Down = 0.0f;
	}
	break;
	case KeyCode::E:
	{
		g_CameraMovement.Up = 0.0f;
	}
	break;
	case KeyCode::R:
	{
		g_CameraMovement.RollCW = 0.0f;
	}
	break;
	case KeyCode::F:
	{
		g_CameraMovement.RollCCW = 0.0f;
	}
	break;
	case KeyCode::ShiftKey:
	{
		g_CameraMovement.TranslateFaster = false;
		g_CameraMovement.RotateFaster = false;
	}
	break;
	}
}

void OnMouseButtonPressed(MouseButtonEventArgs& e)
{
	g_Camera.OnMousePressed(e);

	g_PreviousMousePosition = glm::vec2(e.X, e.Y);
}

void OnMouseButtonReleased(MouseButtonEventArgs& e)
{
	//g_Camera.OnMouseReleased(e);

	glm::vec2 currentMousePosition = glm::vec2(e.X, e.Y);
	float offset = glm::distance(g_PreviousMousePosition, currentMousePosition);
}

void OnMouseMoved(MouseMotionEventArgs& e)
{
	if (e.LeftButton)
	{
		g_Camera.OnMouseMoved(e);
	}
}

void OnMouseWheel(MouseWheelEventArgs& e)
{
	//float fPivot = g_Camera.GetPivotDistance();
	//fPivot -= e.WheelDelta * (g_CameraMovement.TranslateFaster ? 1.0f : 0.1f);
	//g_Camera.SetPivotDistance(fPivot);
}