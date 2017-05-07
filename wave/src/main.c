#include <dusk/dusk.h>
#include <stdio.h>

typedef struct
{
    dusk_Alignas(16) vec3f_t light_pos;
    dusk_Alignas(16) vec3f_t camera_pos;
    dusk_Alignas(4) float time;
    dusk_Alignas(4) float height;
    dusk_Alignas(4) float width;
} wave_data_t;

int           wave_data_index;
wave_data_t   wave_data;
dusk_shader_t wave_shader;

// int           floor_data_index;
// wave_data_t   floor_data;
dusk_shader_t floor_shader;

dusk_model_t wave_model;
dusk_model_t floor_model;

void update(dusk_frame_info_t * finfo)
{
    wave_data.time = fmod(wave_data.time + finfo->elapsed_time / 300.0f, M_PI * 2.0f);

    dusk_shader_set_data(&wave_shader, wave_data_index, (void *)&wave_data);
}

void render()
{
    dusk_model_render(&floor_model);
    dusk_model_render(&wave_model);
}

int main(int argc, char ** argv)
{
    dusk_settings_t settings = DUSK_DEFAULT_SETTINGS;
    settings.window_size     = (vec2u_t){{1024, 768}};
    settings.window_title    = "Wave";

    dusk_callbacks_t callbacks = {
        .update = &update, .render = &render,
    };

    dusk_init(argc, argv, &settings, &callbacks);
    dusk_print_versions();

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    dusk_camera_t * camera = dusk_get_current_camera();
    dusk_camera_set_up(camera, (vec3f_t){{0.0f, 1.0f, 0.0f}});
    dusk_camera_set_pos(camera, (vec3f_t){{100.0f, 30.0f, 100.0f}});
    dusk_camera_look_at(camera, (vec3f_t){{50.0f, -10.0f, 50.0f}});

    dusk_camera_update(camera);

    dusk_shader_file_t wave_files[] = {
        {GL_VERTEX_SHADER, "assets/wave.vs.glsl"},
        {GL_FRAGMENT_SHADER, "assets/wave.fs.glsl"},
        {0, NULL},
    };

    dusk_shader_file_t floor_files[] = {
        {GL_VERTEX_SHADER, "assets/floor.vs.glsl"},
        {GL_FRAGMENT_SHADER, "assets/floor.fs.glsl"},
        {0, NULL},
    };

    vec3f_t cam_pos = dusk_camera_get_pos(camera);
    vec3f_copy(&wave_data.light_pos, &cam_pos);
    vec3f_copy(&wave_data.camera_pos, &cam_pos);
    wave_data.time   = 0;
    wave_data.height = 1.3f;
    wave_data.width  = 0.4f;

    dusk_shader_init(&wave_shader, wave_files);
    dusk_track_static_resource(DUSK_RSC_SHADER, &wave_shader);
    wave_data_index =
        dusk_shader_add_data(&wave_shader, "WaveData", (void *)&wave_data, sizeof(wave_data_t));

    dusk_shader_init(&floor_shader, floor_files);
    dusk_track_static_resource(DUSK_RSC_SHADER, &floor_shader);
    // floor_data_index =
    //    dusk_shader_add_data(&floor_shader, "WaveData", (void *)&floor_data, sizeof(wave_data_t));

    dusk_material_t floor_mat;
    dusk_track_static_resource(DUSK_RSC_MATERIAL, &floor_mat);
    dusk_material_init(&floor_mat, (vec4f_t){{0.0f, 0.0f, 0.0f, 1.0f}},
                       (vec4f_t){{0.0f, 0.0f, 0.0f, 1.0f}}, (vec4f_t){{0.0f, 0.0f, 0.0f, 1.0f}}, 2,
                       0.0f, NULL, "assets/floor_diff.jpg", NULL, NULL);

    dusk_mesh_t floor_mesh;
    dusk_mesh_create_plane(&floor_mesh, &floor_mat, &floor_shader, 10, 10, 100.0f, 100.0f);
    dusk_track_static_resource(DUSK_RSC_MESH, &floor_mesh);

    dusk_mesh_t wave_mesh;
    dusk_mesh_create_plane(&wave_mesh, NULL, &wave_shader, 100, 100, 100.0f, 100.0f);
    dusk_track_static_resource(DUSK_RSC_MESH, &wave_mesh);

    dusk_mesh_t * floor_meshes = { &floor_mesh };
    dusk_model_init(&floor_model, 1, &floor_meshes, &floor_shader);
    dusk_track_static_resource(DUSK_RSC_MODEL, &floor_model);

    dusk_mesh_t * wave_meshes = { & wave_mesh };
    dusk_model_init(&wave_model, 1, &wave_meshes, &wave_shader);
    dusk_track_static_resource(DUSK_RSC_MODEL, &wave_model);

    dusk_model_set_pos(&floor_model, (vec3f_t){{0.0f, -10.0f, 0.0f}});

    dusk_run();
    dusk_term();

    return 0;
}
