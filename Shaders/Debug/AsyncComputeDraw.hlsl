#include "Shaders/Common/UniformGraphic.hlsl"
#include "Shaders/Common/InputGraphic.hlsl"

[[vk::binding(0, 5)]]
uniform Texture2D asyncTexture;

struct FragmentOutput
{
	[[vk::location(0)]] float4	color : SV_Target0;
};

FragmentOutput main(FragmentInput i)
{
	FragmentOutput	o;

	// o.color = float4(1, 1, 0, 1);
	o.color = float4(asyncTexture.SampleLevel(trilinearClamp, i.uv, 0));

	return o;
}
