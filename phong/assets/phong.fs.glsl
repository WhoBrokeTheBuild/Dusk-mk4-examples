#version 330 core

in vec4 position;
in vec4 normal;
in vec2 texcoord;

in vec3 light_dir;
in vec3 view_dir;

out vec4 o_color;

void main()
{
    vec3 color = vec3(0.102, 0.537, 0.133);

    vec3 ambient = 0.1 * color;

    float diff    = max(dot(normal.xyz, light_dir), 0.0);
    vec3  diffuse = diff * color;

    vec3  halfway_dir = normalize(light_dir + view_dir);
    float spec        = pow(max(dot(normal.xyz, halfway_dir), 0.0), 16);
    vec3  specular    = spec * color;

    o_color = vec4(ambient + diffuse + specular, 1.0);
}
