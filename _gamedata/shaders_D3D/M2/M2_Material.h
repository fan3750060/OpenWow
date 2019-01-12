struct M2_Material
{
	uint gIsAnimated;
	uint gBonesMaxInfluences;
	uint gBlendMode;
	float pad;
	float4x4 Bones[200];
    //-------------------------- ( 16 bytes )
};