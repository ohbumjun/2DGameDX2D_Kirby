#include "ShaderInfo.fx"


struct VertexPos
{
	// ���� �ڿ� : ���������̸� + ��ȣ �� �����Ѵ�.
	// ��ȣ�� �Ⱥ��̸� 0���� �����ȴ�.
	float3	Pos : POSITION;	// Vector3Ÿ��.
	uint InstanceID : SV_InstanceID;
};

struct VertexPosOutput
{
	// SV�� ������ System Value�̴�. �� ���� �������Ϳ� ���常 �ϰ�
	// ������ ����ϸ� �ȵȴ�.
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