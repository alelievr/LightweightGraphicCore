#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler;
layout(binding = 2) uniform UniformBufferObject
{
	float time;
} ubo;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;


layout(location = 0) out vec4 outColor;

void main()
{
	//outColor = vec4(fragColor, 1.0);
	outColor = vec4(textureLod(texSampler, fragTexCoord, 7).rgb, 1.0);
}

