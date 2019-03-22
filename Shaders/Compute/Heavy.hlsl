#include "Shaders/Common/InputCompute.hlsl"

#define ITER 200

// auto generate bindings ?
RWTexture2D<half4>	fractal;

float2 hash(float2 x)
{
    const float2 k = float2(0.3183099, 0.3678794);
    x = x * k + k.yx;
    return -1.0 + 2.0 * frac(16.0 * k * frac(x.x * x.y * (x.x + x.y)));
}

float noise(float2 p)
{
    float2 i = floor(p);
    float2 f = frac(p);

	float2 u = f*f*(3.0-2.0*f);

    return lerp(lerp(dot(hash(i + float2(0.0,0.0)), f - float2(0.0,0.0)),
                     dot(hash(i + float2(1.0,0.0)), f - float2(1.0,0.0)), u.x),
                lerp(dot(hash(i + float2(0.0,1.0)), f - float2(0.0,1.0)),
                     dot(hash(i + float2(1.0,1.0)), f - float2(1.0,1.0)), u.x), u.y);
}

// Warning: OSX can't have local work group greater than 1024 threads
[numthreads(16, 16, 1)]
void main(ComputeInput input)
{
	float2 uv = input.dispatchThreadId.xy / 4096.0;
	// uint2 id = input.dispatchThreadId;
	// fractal[id.xy] = float4(id.x & id.y, (id.x & 15)/15.0, (id.y & 15)/15.0, 0.0);
	float4 acc = 0;
	for (int i = 0; i < ITER; i++)
	{
		acc += half4(
			noise(uv * 10),
			noise(uv * 20),
			noise(uv * 30),
			1
		);
	}
	fractal[input.dispatchThreadId.xy] = acc;
}
