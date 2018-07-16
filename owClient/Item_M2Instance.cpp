#include "stdafx.h"

// General
#include "Item_M2Instance.h"

CItem_M2Instance::CItem_M2Instance(CM2_Base_Instance* _parent, M2* _model, const CM2_Part_Attachment* _attach) :
	CM2_Base_Instance(_parent, _model),
	m_Attached(_attach)
{
	CalculateMatrix();
	InitLocal();

	//setScale(10.0f);
	setVisible(true);
}

void CItem_M2Instance::CalculateMatrix(bool _isRotationQuat)
{
	mat4 relMatrix;
	relMatrix.translate(m_Attached->getBone()->getPivot());

	setAbsTrans(getParent()->getAbsTrans() * m_Attached->getBone()->getTransformMatrix() * relMatrix);
}

bool CItem_M2Instance::PreRender3D()
{
	return true;
}

void CItem_M2Instance::Render3D()
{
	CalculateMatrix();

	CM2_Base_Instance::Render3D();
}
