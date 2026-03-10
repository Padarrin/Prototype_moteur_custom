cbuffer cbPerObject : register(b0)
{
	float4x4 gOrthoProj; 
};

Texture2D gSpriteSheet : register(t0);
SamplerState gsamPoint : register(s0);

struct VertexIn
{
	float2 PosL  : POSITION;
    float2 TexC : TEXCOORD;
    float4 Color : COLOR;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
    float2 TexC : TEXCOORD;
    float4 Color : COLOR;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    
    vout.PosH = mul(float4(vin.PosL, 0.0f, 1.0f), gOrthoProj);
    vout.Color = vin.Color;
    vout.TexC = vin.TexC;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    float4 color = gSpriteSheet.Sample(gsamPoint, pin.TexC);
   
    clip(color.r - 0.1f);
    
    return color * pin.Color;
    //return float4(1.0f, 0.0f, 0.0f, 1.0f);
}


