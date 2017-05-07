#version 330 core

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

uniform sampler2D ambient_map;
uniform sampler2D diffuse_map;
uniform sampler2D specular_map;
uniform sampler2D bump_map;

in vec4 p_position;
in vec4 p_normal;
in vec2 p_texcoord;

in vec3 light_dir;
in vec3 view_dir;

out vec4 o_color;

void main()
{
    vec3 ambient, diffuse, specular;
    vec4 normal;

    normal = p_normal;
    if (mesh_data.has_bump_map)
    {
        normal = model_data.model * (texture(bump_map, p_texcoord).rgba * 2.0 - 1.0);
    }

    if (mesh_data.has_ambient_map)
    {
        ambient = texture(ambient_map, p_texcoord).rgb;
    }
    else
    {
        ambient = mesh_data.ambient.rgb;
    }

    if (mesh_data.has_diffuse_map)
    {
        diffuse = texture(diffuse_map, p_texcoord).rgb;
    }
    else
    {
        float diff = max(0.0, dot(normal.xyz, light_dir));
        diffuse    = diff * mesh_data.diffuse.rgb;
    }

    vec3  halfway_dir = normalize(light_dir + view_dir);
    float spec        = max(0.0, dot(normal.xyz, halfway_dir));
    if (mesh_data.shininess > 0)
    {
        spec = pow(spec, mesh_data.shininess);
    }

    if (mesh_data.has_specular_map)
    {
        specular = spec * texture(specular_map, p_texcoord).rgb;
    }
    else
    {
        specular = vec3(spec);
    }

    o_color = vec4(ambient + diffuse + specular, 1.0);
}
