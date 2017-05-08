#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;

layout(std140) uniform LightData
{
    vec3 light_pos;
    vec3 camera_pos;
}
light_data;

layout(std140) uniform ModelData
{
    mat4 model;
    mat4 view;
    mat4 proj;
    mat4 mvp;
}
model_data;

layout(std140) uniform MeshData
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    float shininess;
    float dissolve;

    bool has_ambient_map;
    bool has_diffuse_map;
    bool has_specular_map;
    bool has_bump_map;
}
mesh_data;

out vec4 p_position;
out vec4 p_normal;
out vec2 p_texcoord;

out vec3 light_dir;
out vec3 view_dir;

void main()
{
    p_texcoord = vec2(in_texcoord.x, 1 - in_texcoord.y);
    p_position = model_data.model * vec4(in_position, 1.0);
    p_normal   = model_data.model * vec4(in_normal, 1.0);

    light_dir = light_data.light_pos - p_position.xyz;
    light_dir = normalize(light_dir);

    view_dir = light_data.camera_pos - p_position.xyz;
    view_dir = normalize(view_dir);

    gl_Position = model_data.mvp * vec4(in_position, 1.0);
}
