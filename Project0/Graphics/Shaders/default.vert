#version 450 core

layout (location = 0) in vec2 position;

out vec3 out_color;

uniform vec3 color;

void main()
{
	gl_Position = vec4(position, 0.0, 1.0);
	out_color = color;
}