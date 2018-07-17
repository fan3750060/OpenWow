#pragma once

class CItem_M2Instance : public CM2_Base_Instance
{
public:
	CItem_M2Instance(CM2_Base_Instance* _parent, M2* _model);

	void AddVisualEffect(CM2_Base_Instance* _visualEffect);

	// IRenderable3D
	void Render3D() override;

private:
	vector<SharedPtr<CM2_Base_Instance>> m_VisualEffects;
};