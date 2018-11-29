#ifndef UNIFORMS
# define UNIFORMS

struct LWGC_PerFrame
{
	float4		time; // x: time, y: sin(time), z: cos(time), z: deltaTime
};

struct LWGC_PerCamera
{
	float4x4	projection;
	float4x4	view;
	float4		positionWS;
	float4		screenSize; // xy: screen size in pixel, zw: 1 / screenSize
};

struct LWGC_PerObject
{
	float4x4	model;
};

struct LWGC_PerMaterial
{
	float4		albedo;
};

[[vk::binding(0, 0)]]
ConstantBuffer< LWGC_PerFrame >	frame;

[[vk::binding(0, 1)]]
ConstantBuffer< LWGC_PerCamera> camera;

[[vk::binding(0, 2)]]
ConstantBuffer< LWGC_PerObject > object;

[[vk::binding(0, 3)]]
ConstantBuffer< LWGC_PerMaterial > material;

[[vk::binding(1, 3)]]
uniform SamplerState trilinearClamp;

[[vk::binding(2, 3)]] uniform Texture2D	albedoMap;
[[vk::binding(3, 3)]] uniform Texture2D	normalMap;
[[vk::binding(4, 3)]] uniform Texture2D	heightMap;
[[vk::binding(5, 3)]] uniform Texture2D	smoothnessMap;

#endif
