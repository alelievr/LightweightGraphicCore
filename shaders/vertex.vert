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

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec3 inColor;
layout(location = 4) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

out gl_PerVertex {
	vec4 gl_Position;
};

void main() {
    gl_Position = LWGC_PerCamera.projection * LWGC_PerCamera.view * LWGC_PerObject.model * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}
