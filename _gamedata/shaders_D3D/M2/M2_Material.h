struct M2_Material
{
	uint gIsAnimated;
	uint gBonesMaxInfluences;
	uint gBlendMode;
	uint gShader;
	// 16 bytes
	
	uint  gColorEnable;
	uint  gTextureWeightEnable;
	float gTextureWeight;
    uint  gTextureAnimEnable;
	// 16 bytes
	
    float4x4 gTextureAnimMatrix;
	// 64 bytes
	
	float4   gColor;
	// 16 bytes
	
	float4x4 Bones[200];
	// 64 * 200 bytes
};