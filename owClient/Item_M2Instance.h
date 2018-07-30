#pragma once

class CItem_M2Instance : public CM2_Base_Instance
{
public:
	CItem_M2Instance(CM2_Base_Instance* _parent, SmartM2Ptr _model);

	void AddVisualEffect(std::shared_ptr<CM2_Base_Instance> _visualEffect);

	// IRenderable3D
	void Render3D() override;

private:
	vector<std::shared_ptr<CM2_Base_Instance>> m_VisualEffects;
};