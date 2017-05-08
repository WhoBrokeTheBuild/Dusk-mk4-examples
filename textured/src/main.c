#include <dusk/dusk.h>
#include <stdio.h>
#include <time.h>

typedef struct
{
    vec3f_t light_pos;
    vec3f_t camera_pos;

} textured_data_t;

int             textured_data_index;
textured_data_t textured_data;
dusk_shader_t   textured_shader;

dusk_model_t * globe;

void update(dusk_frame_info_t * finfo, SDL_Event * ev)
{
    vec3f_t rot = dusk_model_get_rot(globe);
    rot.y += GLMM_RAD(0.3f * finfo->delta);
    if (rot.y > (2.0f * GLMM_PI))
        rot.y = 0.0f;
    dusk_model_set_rot(globe, rot);

#ifndef _MSC_VER
    struct timespec ts;
    ts.tv_sec  = 0;
    ts.tv_nsec = 10000;
    nanosleep(&ts, NULL);
#endif
}

void render()
{
    dusk_model_render(globe);
}

int main(int argc, char ** argv)
{
    dusk_settings_t settings = DUSK_DEFAULT_SETTINGS;
    settings.window_size     = (vec2u_t){{1024, 768}};
    settings.window_title    = "Textured";

    dusk_callbacks_t callbacks = {
        .update = &update, .render = &render,
    };

    dusk_init(argc, argv, &settings, &callbacks);
    dusk_print_versions();

    dusk_camera_t * camera = dusk_get_current_camera();
    dusk_camera_set_up(camera, (vec3f_t){{0.0f, 1.0f, 0.0f}});
    dusk_camera_set_pos(camera, (vec3f_t){{2.0f, 2.0f, 2.0f}});
    dusk_camera_look_at(camera, (vec3f_t){{0.0f, 0.0f, 0.0f}});

    dusk_camera_update(camera);

    vec3f_t cam_pos = dusk_camera_get_pos(camera);
    vec3f_copy(&textured_data.light_pos, &(vec3f_t){{5.0f, 5.0f, 5.0f}});
    vec3f_copy(&textured_data.camera_pos, &cam_pos);

    dusk_shader_file_t textured_files[] = {
        {GL_VERTEX_SHADER, "assets/textured.vs.glsl"},
        {GL_FRAGMENT_SHADER, "assets/textured.fs.glsl"},
        {0, NULL},
    };

    dusk_shader_init(&textured_shader, textured_files);
    dusk_track_static_resource(DUSK_RSC_SHADER, &textured_shader);
    textured_data_index = dusk_shader_add_data(&textured_shader, "LightData",
                                               (void *)&textured_data, sizeof(textured_data_t));

    globe = dusk_load_model_from_file("../assets/globe.dmfz", &textured_shader);

    dusk_run();
    dusk_term();

    return 0;
}
