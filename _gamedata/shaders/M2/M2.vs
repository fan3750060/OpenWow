#version 330

// Vertex attrib
layout(location = 0) in vec3 position;
layout(location = 1) in uint blendWeight;
layout(location = 2) in uint blendIndices;
layout(location = 3) in vec3 normal;
layout(location = 4) in vec2 tc0;
layout(location = 5) in vec2 tc1;

// Uniforms
uniform mat4 gProjView;
uniform mat4 gWorld;

// Bones
uniform bool gIsAnimated;
uniform mat4 gBones[200];
uniform  int gBonesMaxInfluences; 

// Texture animating
uniform bool gTextureAnimEnable;
uniform mat4 gTextureAnimMatrix;

// Out
out struct VSOutput
{
	vec3 position;
	vec3 normal;
	vec2 tc0;
	vec2 tc1;
} VSout;

void main(void)
{
	vec4 newVertex = vec4(0.0f);
	
	if (gIsAnimated && gBonesMaxInfluences > 0)
	{
		uint blendWeights[4];
		blendWeights[0] = (blendWeight & 0xFF000000u >> 24) & 0x000000FFu;
		blendWeights[1] = (blendWeight & 0x00FF0000u >> 16) & 0x000000FFu;
		blendWeights[2] = (blendWeight & 0x0000FF00u >>  8) & 0x000000FFu;
		blendWeights[3] = (blendWeight & 0x000000FFu      ) & 0x000000FFu;
			
		uint blendIndicess[4];
		blendIndicess[0] = (blendIndices & 0xFF000000u >> 24) & 0x000000FFu;
		blendIndicess[1] = (blendIndices & 0x00FF0000u >> 16) & 0x000000FFu;
		blendIndicess[2] = (blendIndices & 0x0000FF00u >>  8) & 0x000000FFu;
		blendIndicess[3] = (blendIndices & 0x000000FFu      ) & 0x000000FFu;

		for (int i = 0; i < gBonesMaxInfluences; i++)
		{
			if (blendWeights[i] > 0u)
			{
				newVertex += gBones[blendIndicess[i]] * vec4(position, 1.0f) * (float(blendWeights[i]) / 255.0f);
			}
		}
	}
	else
	{
		newVertex = vec4(position, 1.0f);
	}
		
	gl_Position = gProjView * gWorld * newVertex;

	VSout.position = (gWorld * vec4(newVertex.xyz, 1.0f)).xyz;
	VSout.normal = (gWorld * vec4(normal.xyz, 0.0)).xyz;
	
	if (gTextureAnimEnable)
	{
		VSout.tc0 = (gTextureAnimMatrix * vec4(tc0, 1.0f, 1.0f)).xy;
		VSout.tc1 = (gTextureAnimMatrix * vec4(tc1, 1.0f, 1.0f)).xy;
	}
	else
	{
		VSout.tc0 = tc0;
		VSout.tc1 = tc1;
	}
};