#include "Shaders/Common/InputCompute.hlsl"
#include "Shaders/Common/UniformStructs.hlsl"

[vk::binding(0, 0)]
ConstantBuffer< LWGC_PerFrame > frame;

[vk::binding(0, 1)]
uniform RWTexture2D< half4 > proceduralTexture;

[vk::binding(1, 1)]
uniform RWStructuredBuffer< float >	f;

[numthreads(8, 8, 1)]
void        main(ComputeInput i)
{
	// f[i.groupThreadId.x] = 42;
	proceduralTexture[i.dispatchThreadId.xy] = half4(1, .25, 0, 1);
}
