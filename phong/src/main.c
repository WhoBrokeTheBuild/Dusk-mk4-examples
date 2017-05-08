#include <dusk/dusk.h>
#include <stdio.h>

typedef struct
{
    vec3f_t light_pos;
    vec3f_t camera_pos;

} phong_data_t;

int           phong_data_index;
phong_data_t  phong_data;
dusk_shader_t phong_shader;

dusk_model_t * monkey;

void update(dusk_frame_info_t * finfo, SDL_Event * ev)
{
    vec3f_t rot = dusk_model_get_rot(monkey);
    rot.y += GLMM_RAD(2.0f * finfo->delta);
    if (rot.y > (2.0f * GLMM_PI))
        rot.y = 0.0f;
    dusk_model_set_rot(monkey, rot);
}

void render()
{
    dusk_model_render(monkey);
}

int main(int argc, char ** argv)
{
    dusk_settings_t settings = DUSK_DEFAULT_SETTINGS;
    settings.window_size     = (vec2u_t){{640, 480}};
    settings.window_title    = "Phong";

    dusk_callbacks_t callbacks = {
        .update = &update, .render = &render,
    };

    dusk_init(argc, argv, &settings, &callbacks);
    dusk_print_versions();

    vec3f_t cam_pos = dusk_camera_get_pos(dusk_get_current_camera());
    vec3f_copy(&phong_data.light_pos, &cam_pos);
    vec3f_copy(&phong_data.camera_pos, &cam_pos);

    dusk_shader_file_t phong_files[] = {
        {GL_VERTEX_SHADER, "assets/phong.vs.glsl"},
        {GL_FRAGMENT_SHADER, "assets/phong.fs.glsl"},
        {0, NULL},
    };

    dusk_shader_init(&phong_shader, phong_files);
    dusk_track_static_resource(DUSK_RSC_SHADER, &phong_shader);
    phong_data_index =
        dusk_shader_add_data(&phong_shader, "PhongData", (void *)&phong_data, sizeof(phong_data_t));

    monkey = dusk_load_model_from_file("../assets/monkey.dmfz", &phong_shader);

    dusk_run();
    dusk_term();

    return 0;
}
