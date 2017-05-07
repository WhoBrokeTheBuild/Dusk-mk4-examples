#version 330 core

layout(std140) uniform TriangleData
{
    vec4 color;
}
data;

out vec4 o_color;

void main()
{
    o_color = data.color;
}
