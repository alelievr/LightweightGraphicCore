#include "InputCompute.hlsl"
#include "UniformStructs.hlsl"

[vk::binding(0, 0)]
ConstantBuffer< LWGC_PerFrame > frame;

[vk::binding(0, 1)]
uniform RWTexture2D< half3 > proceduralTexture;

void        main(ComputeShaderInput i)
{
	proceduralTexture[i.groupThreadId] = half3(1, 1, 0);
}