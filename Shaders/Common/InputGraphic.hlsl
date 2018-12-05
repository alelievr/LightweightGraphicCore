#ifndef INPUT_GRAPHIC
# define INPUT_GRAPHIC

struct	VertexInput
{
	[[vk::location(0)]] float3	position : POSITION;
	[[vk::location(1)]] float3	normal : NORMAL;
	[[vk::location(2)]] float3	tangent : TANGENT;
	[[vk::location(3)]] float3	color : COLOR;
	[[vk::location(4)]] float2	uv : TEXCOORD0;
};

struct	FragmentInput
{
	[[vk::location(0)]] float4	positionWS : SV_Position;
	[[vk::location(1)]] float2	uv : TEXCOORD0;
};

#endif
