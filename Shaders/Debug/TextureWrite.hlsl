#include "Shaders/Common/UniformGraphic.hlsl"
#include "Shaders/Common/InputGraphic.hlsl"

struct FragmentOutput
{
	[[vk::location(0)]] float4	color : SV_Target0;
};

FragmentOutput main(FragmentInput i)
{
	FragmentOutput	o;

	// o.color = float4(1, 1, 0, 1);
	o.color = float4(albedoMap.SampleLevel(trilinearClamp, i.uv, + (frame.time.y + 1) * 5).rgb, 1.0) + 0.1;

	return o;
}