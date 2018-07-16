#pragma once

struct ADT_MDXDef
{
	uint32 nameIndex;
	uint32 uniqueId;
	vec3 position;
	vec3 rotation;
	uint16 scale;
    struct
    {
        uint16 Flag_Biodome : 1;
        uint16 Flag_Shrubbery : 1;
        uint16 : 14;
    } flags;
};

class ADT_MDX_Instance : public CM2_Base_Instance
{
public:
	ADT_MDX_Instance(SceneNode* _parent, M2* _mdxObject, const ADT_MDXDef& _placementInfo);
	virtual ~ADT_MDX_Instance();

	// ISceneNode
	string getObjectInfo() const override { return "@ADT_MDX@" + getM2()->getFilename(); }

	// IRenderable3D
	bool PreRender3D() override;
	void Render3D()override;

public: 
	uint32				m_UniqueId;

public:	// Static
	static void reset();
private:
	static std::set<uint32> m_AlreadyDraw;
};