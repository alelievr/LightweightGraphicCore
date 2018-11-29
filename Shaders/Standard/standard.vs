#version 330

in vec3		vertex;
in vec3		normal;
in vec2		uv;
in vec4		color;
in vec3		tangent;

out vec4	fragmentColor;
out vec2	fragmentUV;

void main(void)
{
   fragmentColor = color;
   fragmentUV = uv;
   gl_Position = vec4(vertex, 1.0);
};
