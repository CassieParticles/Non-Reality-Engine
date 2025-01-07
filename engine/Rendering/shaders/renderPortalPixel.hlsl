struct PSInput
{
	float4 position : SV_Position;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
};

struct PSOutput
{
	float4 colour : SV_Target0;
};

Texture2D cTex : register(t0);
SamplerState sampler0 : register(s0);

PSOutput Main(PSInput input)
{
	PSOutput output;
    
	output.colour = cTex.Sample(sampler0, input.uv);

	return output;
}