#include "..\\_gamedata\\shaders_D3D\\CommonInclude.hlsl"

struct VertexShaderInput
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
};
struct VertexShaderOutput
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

// Uniforms
cbuffer PerObject : register(b0)
{
	float4x4 ModelViewProjection;
}


cbuffer ScreenToViewParams : register( b3 ) // Parameters required to convert screen space coordinates to view space params.
{
    float4x4 InverseProjection;
    float2 ScreenDimensions;
}

cbuffer LightIndexBuffer : register( b4 )
{
    uint LightIndex; // The index of the light in the Lights array.
}

Texture2D PositionTextureWS : register( t0 ); // The position from the world space texture.
Texture2D DiffuseTextureVS  : register( t1 ); // The diffuse color from the view space texture.
Texture2D SpecularTextureVS : register( t2 ); // The specular color from the screen space texture.
Texture2D NormalTextureVS   : register( t3 ); // The normal from the screen space texture.
Texture2D DepthTextureVS    : register( t4 ); // The depth from the screen space texture.

StructuredBuffer<Light> Lights : register(t5);

//
// HELPERS
//

// Convert clip space coordinates to view space
float4 ClipToView( float4 clip )
{
    // View space position.
    float4 view = mul( InverseProjection, clip );
    // Perspecitive projection.
    view = view / view.w;

    return view;
}

// Convert screen space coordinates to view space.
float4 ScreenToView( float4 screen )
{
    // Convert to normalized texture coordinates
    float2 texCoord = screen.xy / ScreenDimensions;

    // Convert to clip space
    float4 clip = float4( float2( texCoord.x, 1.0f - texCoord.y ) * 2.0f - 1.0f, screen.z, screen.w );

    return ClipToView( clip );
}

VertexShaderOutput VS_main(VertexShaderInput IN)
{
	VertexShaderOutput OUT;
	OUT.position = mul(ModelViewProjection, float4(IN.position, 1.0f));
	OUT.texCoord = IN.texCoord;
	return OUT;
}

[earlydepthstencil]
float4 PS_DeferredLighting(VertexShaderOutput IN) : SV_Target
{
    // Everything is in view space.
    float4 eyePos = { 0, 0, 0, 1 };

    int2 texCoord = IN.position.xy;
    float depth = DepthTextureVS.Load( int3( texCoord, 0 ) ).r;

    float4 P = ScreenToView( float4( texCoord, depth, 1.0f ) );

    // View vector
    float4 V = normalize( eyePos - P );

    float4 diffuse = DiffuseTextureVS.Load( int3( texCoord, 0 ) );
    float4 specular = SpecularTextureVS.Load( int3( texCoord, 0 ) );
    float4 N = NormalTextureVS.Load( int3( texCoord, 0 ) );

    // Unpack the specular power from the alpha component of the specular color.
    float specularPower = exp2( specular.a * 10.5f );

    Light light = Lights[LightIndex];
    
    LightMaterial mat = (LightMaterial)0;
    mat.DiffuseColor = diffuse;
    mat.SpecularColor = specular;
    mat.SpecularPower = specularPower;

    LightingResult lit = (LightingResult)0;

    switch ( light.Type )
    {
    case DIRECTIONAL_LIGHT:
        lit = DoDirectionalLight( light, mat, V, P, N );
        break;
    case POINT_LIGHT:
        lit = DoPointLight( light, mat, V, P, N );
        break;
    case SPOT_LIGHT:
        lit = DoSpotLight( light, mat, V, P, N );
        break;
    }

    return (diffuse * lit.Diffuse) + (specular * lit.Specular);
}
