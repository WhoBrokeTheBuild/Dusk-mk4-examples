#version 330 core

in vec3 position;
in vec3 normal;
in vec2 texcoord;

layout(std140) uniform FlatData
{
    vec4 color;
}
flat_data;

out vec4 o_color;

void main()
{
    o_color = flat_data.color;
}
