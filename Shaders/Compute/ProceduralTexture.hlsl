#include "Shaders/Common/InputCompute.hlsl"
#include "Shaders/Common/UniformStructs.hlsl"

[vk::binding(0, 0)]
ConstantBuffer< LWGC_PerFrame > frame;

[vk::binding(0, 1)]
uniform RWTexture2D< half4 > proceduralTexture;

[numthreads(8, 8, 1)]
void        main(ComputeInput i)
{
	float2 uv = i.dispatchThreadId.xy / float2(512);
	proceduralTexture[i.dispatchThreadId.xy] = half4(uv, 1, 1);
}
