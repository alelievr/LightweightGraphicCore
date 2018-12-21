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
	// proceduralTexture[i.dispatchThreadId.xy] = half4(1, .25, 0, 1);

	float x = float(i.dispatchThreadId.x) / float(512);
	float y = float(i.dispatchThreadId.y) / float(512);

	/*
	What follows is code for rendering the mandelbrot set.
	*/
	float2 uv = float2(x,y);
	float n = 0.0;
	float2 c = float2(-.445, 0.0) +  (uv - 0.5)*(2.0+ 1.7*0.2  ), z = float2(0.0);
	const int M = 1280;
	for (int i = 0; i < M; i++)
	{
		z = float2(z.x*z.x - z.y*z.y, 2.*z.x*z.y) + c;
		if (dot(z, z) > 2) break;
		n++;
	}

	// we use a simple cosine palette to determine color:
	// http://iquilezles.org/www/articles/palettes/palettes.htm
	float t = float(n) / float(M);
	float3 d = float3(0.3, 0.3 ,0.5);
	float3 e = float3(-0.2, -0.3 ,-0.5);
	float3 f = float3(2.1, 2.0, 3.0);
	float3 g = float3(0.0, 0.1, 0.0);
	int ii = frame.time.x * 600;
	proceduralTexture[i.dispatchThreadId.xy] = half4( d + e*cos( 6.28318*(f*t+g) + ii) ,1.0);
}
