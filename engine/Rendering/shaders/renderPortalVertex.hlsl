struct VSInput
{
    float3 position : POSITION0;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
};

struct VSOutput
{
    float4 position : SV_Position;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
};

cbuffer CameraMatrix : register(b0)
{
    float4x4 cameraMatrix;
};

cbuffer WorldMatrix : register(b1)
{
    float4x4 worldMatrix;
}

VSOutput Main(VSInput input)
{
    VSOutput output;
    output.position = float4(input.position, 1.0);
    output.position = mul(worldMatrix, output.position); //Into world space
    output.position = mul(cameraMatrix, output.position); //Into clip space
    
    output.normal = input.normal;
    output.normal = mul((float3x3) worldMatrix, output.normal);
    
    //UVs calcualted in pixel shader

    return output;
}