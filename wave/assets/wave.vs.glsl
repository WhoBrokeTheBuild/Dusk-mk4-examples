#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;

layout(std140) uniform ModelData
{
    mat4 model;
    mat4 view;
    mat4 proj;
    mat4 mvp;
}
model_data;

layout(std140) uniform WaveData
{
    vec3  light_pos;
    vec3  camera_pos;
    float time;
    float height;
    float width;
}
wave_data;

out vec3 position;
out vec3 normal;
out vec2 texcoord;

out vec3 light_dir;
out vec3 view_dir;

void main()
{
    position = in_position;
    normal   = in_normal;
    texcoord = in_texcoord;

    position.y = sin(wave_data.width * (position.x / 3 + position.z / 2) + wave_data.time) *
                 wave_data.height / 4.0;
    position.y += cos(wave_data.width * (position.x / 2 + position.z / 3) + wave_data.time) *
                  wave_data.height / 4.0;
    position.y +=
        cos(wave_data.width * (position.x + position.z) + wave_data.time) * wave_data.height / 8.0;
    position.y += cos(wave_data.width / 3 * position.x + wave_data.time) * wave_data.height / 2.0;

    vec3 Nx = vec3(1,
                   wave_data.height * wave_data.width *
                       cos(position.x * wave_data.width + wave_data.time) *
                       cos(position.z * wave_data.width + wave_data.time),
                   0);
    vec3 Nz = vec3(0,
                   wave_data.height * wave_data.width *
                       sin(wave_data.width * position.x + wave_data.time) *
                       sin(position.x * wave_data.width + wave_data.time),
                   1);

    normal = normalize(cross(Nx, Nz));

    vec3 position_worldspace = vec3(model_data.model * vec4(position, 1.0));
    light_dir                = wave_data.light_pos - position_worldspace;
    light_dir                = normalize(light_dir);
    view_dir                 = wave_data.camera_pos - position_worldspace;
    view_dir                 = normalize(view_dir);

    gl_Position = model_data.mvp * vec4(position, 1.0);
}
