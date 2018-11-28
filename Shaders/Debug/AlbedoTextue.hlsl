#include "Shaders/Common/Uniforms.hlsl"

struct FragmentOutput
{
	[[vk::location(0)]] float4	color : SV_Target0;
};

struct	FragmentInput
{
	[[vk::location(0)]] float4	positionWS : SV_Position;
	[[vk::location(1)]] float2	uv : TEXCOORD0;
};

FragmentOutput main(FragmentInput i)
{
	FragmentOutput	o;

	o.color = float4(albedoMap.SampleLevel(trilinearClamp, i.uv * 2, 7).rgb, 1.0);

	return o;
}
