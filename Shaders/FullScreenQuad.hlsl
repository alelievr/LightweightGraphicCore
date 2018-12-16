#include "Shaders/Common/UniformGraphic.hlsl"
#include "Shaders/Common/InputGraphic.hlsl"

// Vulkan NDC coordinates
const float2	screenPositions[6] = {
	float2(1, -1),
	float2(-1, -1),
	float2(1, 1),
	float2(-1, 1),
};

FragmentInput main(int id : SV_VertexID)
{
	FragmentInput	o;

    o.uv = (screenPositions[id] + 1) * 0.5;
	o.positionWS = float4(screenPositions[id].xy, 0.999999492, 1);

	return o;
}
