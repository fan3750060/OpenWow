#pragma once

#include "M2.h"
#include "M2_Skin.h"
#include "M2_SkinSection.h"

// FORWARD BEGIN
class CM2_Builder;
// FORWARD END

/*struct M2_SkinBatch_PriorityPlan_Compare
{
	bool operator() (const CM2_Skin_Batch* left, const CM2_Skin_Batch* right) const
	{
		return left->getPriorityPlan() < right->getPriorityPlan();
	}
};


struct M2_SkinBatch_MeshID_Compare
{
	bool operator() (const CM2_Skin_Batch* left, const CM2_Skin_Batch* right) const
	{
		return left->getSkin().lock()->getProto().meshPartID < right->getSkin().lock()->getProto().meshPartID;
	}
};*/

class CM2_Skin_Builder
{
public:
	CM2_Skin_Builder(const CM2_Builder& _m2Builder, std::weak_ptr<M2> _model, std::shared_ptr<CM2_Skin> _skin, std::shared_ptr<IFile> _file);

	void Load();

	// Loader
	void Step1LoadProfile();
	void Step2InitBatches();

private:
	SM2_SkinProfile				m_SkinProfile;
	std::weak_ptr<CM2_Skin>		m_Skin;
	std::weak_ptr<IFile>		m_F;

	//

	std::vector<SM2_SkinBatch>	m_SkinBatches;

	//--
	const CM2_Builder&  m_M2Builder;
	std::weak_ptr<M2>			m_ParentM2;
};