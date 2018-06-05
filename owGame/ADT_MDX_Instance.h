#pragma once

struct ADT_MDDF
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

class ADT_MDX_Instance : public SceneNode
{
public:
	ADT_MDX_Instance(M2* _mdxObject, ADT_MDDF _placementInfo);
	~ADT_MDX_Instance();

    inline M2* GetMDX() { return m_Object; }

	// IRenderable3D
	void PreRender3D(double _time, double _dTime) override;
	void Render3D()override;

public: 
	SmartM2Ptr	m_Object;
	uint32		m_UniqueId;

public:	// Static
	static void reset();
private:
	static std::set<uint32> m_AlreadyDraw;
};