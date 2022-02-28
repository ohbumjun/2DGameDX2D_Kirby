#include "ShaderInfo.fx"


struct VertexPos
{
	// 변수 뒤에 : 레지스터이름 + 번호 로 지정한다.
	// 번호를 안붙이면 0으로 지정된다.
	float3	Pos : POSITION;	// Vector3타입.
	uint InstanceID : SV_InstanceID;
};

struct VertexPosOutput
{
	// SV가 붙으면 System Value이다. 이 값은 레지스터에 저장만 하고
	// 가져다 사용하면 안된다.
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
	float Opacity : TEXCOORD;
};

struct TileInfoEmpty
{
	matrix matWVP;
	float4 TileColor;
	float Opacity;
	float3 Empty;
};

StructuredBuffer<TileInfoEmpty> g_TileEmptyArray : register(t90);

VertexPosOutput TileMapEmptyVS (VertexPos input)
{
	VertexPosOutput output = (VertexPos)0;

	output.Pos = mul(float4(input.Pos, 1.f), g_TileEmptyArray[input.InstanceID].matWVP);

	output.Color = g_TileEmptyArray[input.InstanceID].TileColor;
	output.Opacity = g_TileEmptyArray[input.InstanceID].Opacity;

	return output;
}

PSOutput_Single TileMapEmptyPS(VertexPosOutput input)
{
	PSOutput_Single	output = (PSOutput_Single)0;

	output.Color.rgb = input.Color.rgb;
	output.Color.a = input.Color.a * input.Opacity;

	return output;
}