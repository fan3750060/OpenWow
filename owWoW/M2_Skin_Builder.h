#pragma once

#include "MDX.h"
#include "M2_Skin.h"

class CM2_Skin_Builder
{
public:
	CM2_Skin_Builder(MDX* _model, const SM2_SkinProfile& _skinProto, CM2_Skin* _skin, IFile* _file);
	~CM2_Skin_Builder();

	void Load();

	// Loader
	void Step1LoadProfile();
	void Step2InitBatches();

	void StepBuildGeometry();

private:
	MDX*					m_MDX;
	const SM2_SkinProfile&	m_SkinProto;
	CM2_Skin*				m_Skin;
	IFile*					m_F;				// Don't delete this!

	//

	vector<uint16>			verticesIndexes;
	vector<uint16>			indexesIndexes;
	vector<SM2_SkinBones>	bonesIndexes;
	vector<SM2_SkinSection> skins;
	vector<SM2_SkinBatch>	batches;
};