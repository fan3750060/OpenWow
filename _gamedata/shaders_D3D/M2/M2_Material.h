struct M2_Material
{
	uint gIsAnimated;
	uint gBonesMaxInfluences;
	float2 pad;
	float4x4 Bones[200];
    //-------------------------- ( 16 bytes )
};