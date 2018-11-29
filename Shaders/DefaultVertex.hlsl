#include "Shaders/Common/Uniforms.hlsl"
#include "Shaders/Common/Inputs.hlsl"

FragmentInput main(VertexInput i, int id : SV_VertexID)
{
	FragmentInput	o;

    o.uv = i.uv;
	o.positionWS = transpose(camera.projection) * transpose(camera.view) * object.model * (float4(i.position.xyz, 1));

	return o;
}
