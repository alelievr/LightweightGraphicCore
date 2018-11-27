#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform a {
	vec4	time; // x: time, y: sin(time), z: cos(time), z: deltaTime
} LWGC_PerFrame;

layout(set = 1, binding = 0) uniform b {
	mat4	projection;
	mat4	view;
	vec4	positionWS;
	vec4	screenSize; // xy: screen size in pixel, zw: 1 / screenSize
} LWGC_PerCamera;

layout(set = 2, binding = 0) uniform c {
	mat4 model;
} LWGC_PerObject;

layout(set = 3, binding = 0) uniform d {
	vec4		albedo;
} LWGC_PerMaterial;

layout(set = 3, binding = 1) uniform sampler trilinearClamp;

layout(set = 3, binding = 2) uniform texture2D	albedoMap;
// layout(binding = 1) uniform sampler2D	normalMap;
// layout(binding = 2) uniform sampler2D	heightMap;
// layout(binding = 3) uniform sampler2D	smoothnessMap;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main()
{
	// outColor = vec4(1, 1, 0, 1);
	// outColor = vec4(LWGC_PerMaterial.albedo.rgb, 1.0);
	outColor = vec4(textureLod(sampler2D(albedoMap, trilinearClamp), fragTexCoord * 2, 5).rgb, 1.0);
}
