#version 330 core

in vec3 position;
in vec3 normal;
in vec2 texcoord;

in float wave_height;

layout(std140) uniform WaveData
{
    vec3  light_pos;
    vec3  camera_pos;
    float time;
    float height;
    float width;
}
wave_data;

out vec4 o_color;

void main()
{
    float wave_height = ((position.y + wave_data.height) / (2.0 * wave_data.height));
    float shine       = wave_height / 3;
    vec4  specular    = vec4(shine);

    o_color = vec4(28 / 255.0, 107 / 255.0, 160 / 255.0, 0.6);
    o_color += specular;
    // o_color = vec4(1, 0, 0, 1);
}
