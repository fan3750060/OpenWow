#pragma once

class CItem_M2Instance : public CM2_Base_Instance
{
public:
	CItem_M2Instance(CM2_Base_Instance* _parent, M2* _model, const CM2_Part_Attachment* _attach);

	// ISceneNode
	virtual void CalculateMatrix(bool _isRotationQuat = false) override;

	// IRenderable3D
	bool PreRender3D() override;
	void Render3D() override;

private:
	const CM2_Part_Attachment* m_Attached;
	
};