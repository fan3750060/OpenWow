#pragma once

class CItem_M2Instance : public CM2_Base_Instance
{
public:
	CItem_M2Instance(std::shared_ptr<M2> _model);

	void AddVisualEffect(std::shared_ptr<CM2_Base_Instance> _visualEffect);

private:
	std::vector<std::shared_ptr<CM2_Base_Instance>> m_VisualEffects;
};