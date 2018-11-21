#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform a {
	vec4	time; // x: time, y: sin(time), z: cos(time), z: deltaTime
} LWGC_PerFrame;

layout(binding = 1) uniform b {
	mat4	projection;
	mat4	view;
	vec4	positionWS;
	vec4	screenSize; // xy: screen size in pixel, zw: 1 / screenSize
} LWGC_PerCamera;

layout(binding = 2) uniform c {
	mat4 model;
} LWGC_PerObject;

layout(binding = 3) uniform d {
	vec4		albedo;
} LWGC_PerMaterial;

layout(binding = 4) uniform sampler2D	albedoMap;
layout(binding = 5) uniform sampler2D	normalMap;
layout(binding = 6) uniform sampler2D	heightMap;
layout(binding = 7) uniform sampler2D	smoothnessMap;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main()
{
	//outColor = vec4(fragColor, 1.0);
	outColor = vec4(textureLod(albedoMap, fragTexCoord, 5).rgb, 1.0);
}

