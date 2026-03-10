//***************************************************************************************
// color.hlsl by Frank Luna (C) 2015 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

cbuffer cbPerObject : register(b0)
{
	float4x4 gWorldViewProj; 
    float time;
};

cbuffer cbMaterial : register(b1)
{
    float4 gDiffuseAlbedo;
    float3 gFresnelR0;
    float gRoughness;
};

struct VertexIn
{
	float3 PosL  : POSITION;
    float4 Color : COLOR;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
    float4 Color : COLOR;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    
    float scale = 1.0f + 0.2f * sin(time * 3.0f);

    float3 animatedPos = vin.PosL * scale;
    
    vout.PosH = mul(float4(animatedPos, 1.0f), gWorldViewProj);

    vout.Color = vin.Color;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    float3 rainbow = 0.5f + 0.5f * cos(time * 2.0f + float3(0.0f, 2.0f, 4.0f));

    float pingpong = sin(time);

   
    float3 finalColor = rainbow;
    
    
    if (pingpong > 0.0f)
    {
       
        finalColor = lerp(rainbow, float3(1.0f, 1.0f, 1.0f), pingpong);
    }
    else
    {
        
        finalColor = lerp(rainbow, float3(0.0f, 0.0f, 0.0f), -pingpong);
    }

 
    finalColor = finalColor * gDiffuseAlbedo.rgb;
    
    return float4(finalColor, gDiffuseAlbedo.a);
    
}


