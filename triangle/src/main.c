#include <dusk/dusk.h>
#include <stdio.h>

dusk_model_t triangle_model;

void render()
{
    dusk_model_render(&triangle_model);
}

int main(int argc, char ** argv)
{
    dusk_settings_t settings = DUSK_DEFAULT_SETTINGS;
    settings.window_size     = (vec2u_t){{640, 480}};
    settings.window_title    = "Triangle";

    dusk_callbacks_t callbacks = {
        .update = NULL, .render = &render,
    };

    dusk_init(argc, argv, &settings, &callbacks);
    dusk_print_versions();

    typedef struct
    {
        float color[4];
    } triangle_data_t;

    dusk_shader_t   triangle_shader;
    triangle_data_t data = {
        .color = {0.0f, 1.0f, 0.0f, 1.0f},
    };

    dusk_shader_file_t triangle_files[] = {
        {GL_VERTEX_SHADER, "assets/triangle.vs.glsl"},
        {GL_FRAGMENT_SHADER, "assets/triangle.fs.glsl"},
        {0, NULL},
    };

    dusk_shader_init(&triangle_shader, triangle_files);
    dusk_track_static_resource(DUSK_RSC_SHADER, &triangle_shader);
    dusk_shader_add_data(&triangle_shader, "TriangleData", (void *)&data, sizeof(triangle_data_t));

    // clang-format off

    static const GLfloat verts[] = {
      -1.0f, -1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
       0.0f,  1.0f, 0.0f,
    };

    // clang-format on

    dusk_mesh_t triangle_mesh;
    dusk_mesh_init(&triangle_mesh, NULL, &triangle_shader, 3, verts, NULL, NULL);
    dusk_track_static_resource(DUSK_RSC_MESH, &triangle_mesh);

    dusk_mesh_t * meshes = {&triangle_mesh};
    dusk_model_init(&triangle_model, 1, &meshes, &triangle_shader);
    dusk_track_static_resource(DUSK_RSC_MODEL, &triangle_model);

    dusk_run();
    dusk_term();

    return 0;
}
