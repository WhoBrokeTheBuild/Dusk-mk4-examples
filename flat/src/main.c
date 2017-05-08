#include <dusk/dusk.h>
#include <stdio.h>

#define MAX_COLOR_CHANGE_DELAY 300
vec4f_t COLORS[] = {
    {{190.0f / 255.0f, 25.0f / 255.0f, 49.0f / 255.0f, 1.0f}},
    {{253.0f / 255.0f, 203.0f / 255.0f, 88.0f / 255.0f, 1.0f}},
    {{120.0f / 255.0f, 177.0f / 255.0f, 89.0f / 255.0f, 1.0f}},
    {{93.0f / 255.0f, 173.0f / 255.0f, 236.0f / 255.0f, 1.0f}},
    {{170.0f / 255.0f, 143.0f / 255.0f, 214.0f / 255.0f, 1.0f}},
};
#define NUM_COLORS (int)(sizeof(COLORS) / sizeof(vec4f_t))

int    color_index        = 0;
double color_change_delay = MAX_COLOR_CHANGE_DELAY;

typedef struct
{
    vec4f_t color;
} flat_data_t;

int           flat_data_index;
flat_data_t   flat_data;
dusk_shader_t flat_shader;

dusk_model_t * cube;

void update(dusk_frame_info_t * finfo, SDL_Event * ev)
{
    vec3f_t rot = dusk_model_get_rot(cube);
    rot.y += GLMM_RAD(2.0f * finfo->delta);
    if (rot.y > GLMM_PI)
        rot.y = 0.0f;
    dusk_model_set_rot(cube, rot);

    color_change_delay -= finfo->elapsed_time;
    if (color_change_delay <= 0)
    {
        color_change_delay = MAX_COLOR_CHANGE_DELAY;
        ++color_index;
        if (color_index >= NUM_COLORS)
        {
            color_index = 0;
        }

        vec4f_copy(&flat_data.color, &COLORS[color_index]);
        dusk_shader_set_data(&flat_shader, flat_data_index, (void *)&flat_data);
    }
}

void render()
{
    dusk_model_render(cube);
}

int main(int argc, char ** argv)
{
    dusk_settings_t settings = DUSK_DEFAULT_SETTINGS;
    settings.window_size     = (vec2u_t){{640, 480}};
    settings.window_title    = "Flat";

    dusk_callbacks_t callbacks = {
        .update = &update, .render = &render,
    };

    dusk_init(argc, argv, &settings, &callbacks);
    dusk_print_versions();

    color_index        = 0;
    color_change_delay = MAX_COLOR_CHANGE_DELAY;
    vec4f_copy(&flat_data.color, &COLORS[color_index]);

    dusk_shader_file_t flat_files[] = {
        {GL_VERTEX_SHADER, "assets/flat.vs.glsl"},
        {GL_FRAGMENT_SHADER, "assets/flat.fs.glsl"},
        {0, NULL},
    };

    dusk_shader_init(&flat_shader, flat_files);
    dusk_track_static_resource(DUSK_RSC_SHADER, &flat_shader);
    flat_data_index =
        dusk_shader_add_data(&flat_shader, "FlatData", (void *)&flat_data, sizeof(flat_data_t));

    cube = dusk_load_model_from_file("../assets/cube.dmfz", &flat_shader);

    dusk_run();
    dusk_term();

    return 0;
}
