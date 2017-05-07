#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;

layout(std140) uniform PhongData
{
    vec3 light_pos;
    vec3 camera_pos;
}
phong_data;

layout(std140) uniform ModelData
{
    mat4 model;
    mat4 view;
    mat4 proj;
    mat4 mvp;
}
model_data;

out vec4 position;
out vec4 normal;
out vec2 texcoord;

out vec3 light_dir;
out vec3 view_dir;

void main()
{
    position = model_data.model * vec4(in_position, 1.0);
    normal   = model_data.model * vec4(in_normal, 1.0);
    texcoord = in_texcoord;

    vec3 position_worldspace = vec3(model_data.model * position);

    light_dir = phong_data.light_pos - position_worldspace;
    light_dir = normalize(light_dir);

    view_dir = phong_data.camera_pos - position_worldspace;
    view_dir = normalize(view_dir);

    gl_Position = model_data.mvp * vec4(in_position, 1.0);
}
