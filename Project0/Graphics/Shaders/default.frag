#version 450 core
in vec3 out_color;

layout (location = 0) out vec4 fragColor;

void main()
{
	fragColor = vec4(1.0, 0.0, 0.0, 1.0);
	fragColor = vec4(out_color, 1.0);
}