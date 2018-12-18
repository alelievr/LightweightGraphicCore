#include "Shaders/Common/UniformGraphic.hlsl"
#include "Shaders/Common/InputGraphic.hlsl"

struct FragmentOutput
{
	[[vk::location(0)]] float4	color : SV_Target0;
};

FragmentOutput main(FragmentInput i)
{
	FragmentOutput	o;

	o.color = float4(i.normalOS * 0.5 + 0.5, 1);

	return o;
}
