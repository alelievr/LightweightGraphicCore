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

struct	VertexInput
{
	[[vk::location(0)]] float3	position : POSITION;
	[[vk::location(1)]] float3	normal : NORMAL;
	[[vk::location(2)]] float3	tangent : TANGENT;
	[[vk::location(3)]] float3	color : COLOR;
	[[vk::location(4)]] float2	uv : TEXCOORD0;
};

struct	FragmentInput
{
	[[vk::location(0)]] float4	positionWS : SV_Position;
	[[vk::location(1)]] float2	uv : TEXCOORD0;
};

FragmentInput main(VertexInput i, int id : SV_VertexID)
{
	FragmentInput	o;

    o.uv = i.uv;
	o.positionWS = transpose(camera.projection) * transpose(camera.view) * object.model * (float4(i.position.xyz, 1));

	return o;
}