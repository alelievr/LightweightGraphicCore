#include "Shaders/Common/UniformGraphic.hlsl"
#include "Shaders/Common/InputGraphic.hlsl"
#include "Shaders/Common/Utils.hlsl"

struct FragmentOutput
{
	[[vk::location(0)]] float4	color : SV_Target0;
};

struct SizeOffset
{
	float4	sizeOffset;
};

struct Atlas
{
	float4	size;
};

[vk::binding(0, 4)]
StructuredBuffer< SizeOffset >	sizeOffsets;
[vk::binding(1, 4)]
ConstantBuffer< Atlas >	atlas;

FragmentOutput main(FragmentInput i)
{
	FragmentOutput	o;

	float4 sizeOffset = sizeOffsets[step(frame.time.y, 0.5)].sizeOffset;
    // sizeOffset.zw += atlas[0].size.zw * 0.5;

	// o.color = float4(1, 1, 0, 1);
	float2 atlasUVs = UvToAtlas(i.uv, sizeOffset);
	o.color = float4(albedoMap.SampleLevel(trilinearClamp, atlasUVs, 0).rgb, 0.5);

	return o;
}
