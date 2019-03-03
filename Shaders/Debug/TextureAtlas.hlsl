#include "Shaders/Common/UniformGraphic.hlsl"
#include "Shaders/Common/InputGraphic.hlsl"

struct AtlasDatas
{
	float4		atlasSize;
	float4		sizeOffsets[16];
};

struct FragmentOutput
{
	[[vk::location(0)]] float4	color : SV_Target0;
};

ConstantBuffer< AtlasDatas >	atlas;

FragmentOutput main(FragmentInput i)
{
	FragmentOutput	o;

	// o.color = float4(1, 1, 0, 1);
	float2 atlasUVs = UvToAtlas(i.uv, atlas.sizeOffsets[0]);
	o.color = float4(albedoMap.SampleLevel(trilinearClamp, atlasUVs, 0));

	return o;
}
