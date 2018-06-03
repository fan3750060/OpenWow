#pragma once

#include "MDX.h"

class CM2_Builder
{
public:
	CM2_Builder(MDX* _model);
	~CM2_Builder();

	void Load();

	// Loader
	void Step1Header();
	void Step2GlobalLoops();
	void Step3Bones();
	void Step4Vertices();
	void Step5ColorAndTextures();
	void Step6Misc();
	void Step7Particles();
	void Step8Skins();


	//

	void SetAnimated();
	bool IsAnimated() const { return m_MDX->m_IsAnimated; }

private:
	MDX*					m_MDX;
	IFile*					m_F;
	SM2_Header*				m_Header;

	// !! Don't delete this, it's temp data !! //

	// Global loop, animation and bones
	vector<SM2_Loop>*		m_GlobalLoops;

	// Bones
	SM2_Bone*				m_M2Bones;

	// Vertices
	vector<SM2_Vertex>		m_Vertexes;

	// Skins
	SM2_SkinProfile*		m_Skins;

	// Textures
	SM2_Texture*			m_Textures;
	SM2_TextureWeight*		m_TexturesWeight;
	SM2_TextureTransform*	m_TexturesTransform;
};