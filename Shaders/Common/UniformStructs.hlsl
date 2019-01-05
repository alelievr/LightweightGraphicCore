#ifndef UNIFORMS_STRUCTS
# define UNIFORMS_STRUCTS

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

struct LWGC_GizmoData
{
	float4	color;
	int		colorMode;
};

#endif