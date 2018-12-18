#include "Shaders/Common/UniformGraphic.hlsl"
#include "Shaders/Common/InputGraphic.hlsl"

FragmentInput main(VertexInput i, int id : SV_VertexID, int elementID : SV_InstanceID)
{
	FragmentInput	o;

    o.uv = i.uv;
	float4x4 mvp = camera.projection * camera.view * object.model;
	o.positionWS = mul(float4(i.position.xyz, 1), mvp);
	o.normalOS = i.normal;

	return o;
}
