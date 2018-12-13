#include "Shaders/Common/InputCompute.hlsl"
#include "Shaders/Common/UniformStructs.hlsl"

[vk::binding(0, 0)]
ConstantBuffer< LWGC_PerFrame > frame;

[vk::binding(0, 1)]
uniform RWTexture2D< half3 > proceduralTexture;

[numthreads(8, 8, 1)]
void        main(void)
{
	// proceduralTexture[i.groupThreadId] = half3(1, 1, 0);
}
