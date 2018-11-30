#include "Shaders/Common/Uniforms.hlsl"
#include "Shaders/Common/Inputs.hlsl"

FragmentInput main(VertexInput i, int id : SV_VertexID)
{
	FragmentInput	o;

    o.uv = i.uv;
	float4x4 mvp = transpose(camera.projection) * transpose(camera.view) * transpose(object.model);
	o.positionWS = mul(float4(i.position.xyz + float3(0, 0, 0), 1), mvp);

	return o;
}
