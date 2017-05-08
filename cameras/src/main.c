#include <dusk/dusk.h>
#include <stdio.h>

typedef struct
{
    vec3f_t light_pos;
    vec3f_t camera_pos;

} light_data_t;

int           light_data_index;
light_data_t  light_data;
dusk_shader_t shader;

dusk_model_t * model;

#define CAMERA_COUNT 10
int camera_index = 0;
dusk_camera_t cameras[CAMERA_COUNT];

void update(dusk_frame_info_t * finfo, SDL_Event * ev)
{
    dusk_camera_t * camera = dusk_get_current_camera();

    // vec3f_t rot = dusk_model_get_rot(model);
    // rot.y += GLMM_RAD(2.0f * finfo->delta);
    // if (rot.y > GLMM_PI)
    //    rot.y = 0.0f;
    // dusk_model_set_rot(model, rot);

    vec3f_t cam_pos = dusk_camera_get_pos(camera);
    vec3f_copy(&light_data.camera_pos, &cam_pos);
    dusk_shader_set_data(&shader, light_data_index, &light_data);

    if (NULL != ev)
    {
        float speed = 5.0f;

        switch (ev->type)
        {
        case SDL_MOUSEMOTION:

            if (ev->motion.state & SDL_BUTTON(SDL_BUTTON_LEFT))
            {
                dusk_camera_change_pitch(camera, GLMM_RAD(ev->motion.yrel * 0.1f));
                dusk_camera_update(camera);
                dusk_camera_change_yaw(camera, GLMM_RAD(ev->motion.xrel * 0.1f));
            }

            break;
        case SDL_KEYDOWN:

            switch (ev->key.keysym.sym)
            {
            case SDLK_LEFT:
            case SDLK_a:

                dusk_camera_move(camera, (vec3f_t){{ -speed, 0.0f, 0.0f }});

                break;
            case SDLK_RIGHT:
            case SDLK_d:

                dusk_camera_move(camera, (vec3f_t){{ speed, 0.0f, 0.0f }});

                break;
            case SDLK_UP:
            case SDLK_w:

                dusk_camera_move(camera, (vec3f_t){{ 0.0f, 0.0f, -speed }});

                break;
            case SDLK_DOWN:
            case SDLK_s:

                dusk_camera_move(camera, (vec3f_t){{ 0.0f, 0.0f, speed }});

                break;
            case SDLK_PAGEUP:
            case SDLK_q:

                dusk_camera_move(camera, (vec3f_t){{ 0.0f, speed, 0.0f }});

                break;
            case SDLK_PAGEDOWN:
            case SDLK_e:

                dusk_camera_move(camera, (vec3f_t){{ 0.0f, -speed, 0.0f }});

                break;
            }

            break;
        case SDL_MOUSEWHEEL:

            camera_index += ev->wheel.y;
            if (camera_index < 0)
            {
                camera_index = CAMERA_COUNT - 1;
            }
            else if (camera_index >= CAMERA_COUNT)
            {
                camera_index = 0;
            }

            dusk_set_current_camera(&cameras[camera_index]);

            break;
        }
    }

    dusk_camera_update(camera);
}

void render()
{
    dusk_model_render(model);
}

int main(int argc, char ** argv)
{
    dusk_settings_t settings = DUSK_DEFAULT_SETTINGS;
    settings.window_size     = (vec2u_t){{1024, 768}};
    settings.window_title    = "Cameras";

    dusk_callbacks_t callbacks = {
        .update = &update, .render = &render,
    };

    dusk_init(argc, argv, &settings, &callbacks);
    dusk_print_versions();

    dusk_camera_t * camera;

    for (int i = 0; i < CAMERA_COUNT; ++i)
    {
        camera = &cameras[i];
        dusk_camera_init(camera);

        dusk_track_static_resource(DUSK_RSC_CAMERA, camera);
        dusk_add_camera(camera);

        dusk_camera_set_aspect(camera, settings.window_size.x, settings.window_size.y);
        dusk_camera_set_clip(camera, 0.1f, 1000.0f);
        dusk_camera_set_fov(camera, GLMM_RAD(45.0f));
        dusk_camera_set_up(camera, (vec3f_t){{0.0f, 1.0f, 0.0f}});

        dusk_camera_set_pos(camera, (vec3f_t){{
            (float)rand() / (float)(RAND_MAX / 5.0f),
            (float)rand() / (float)(RAND_MAX / 5.0f),
            (float)rand() / (float)(RAND_MAX / 5.0f)
        }});

        dusk_camera_look_at(camera, (vec3f_t){{
            (float)rand() / (float)(RAND_MAX / 5.0f),
            (float)rand() / (float)(RAND_MAX / 5.0f),
            (float)rand() / (float)(RAND_MAX / 5.0f)
        }});

        dusk_camera_update(camera);
    }

    dusk_set_current_camera(&cameras[0]);

    vec3f_t cam_pos = dusk_camera_get_pos(&cameras[0]);
    vec3f_copy(&light_data.light_pos, &(vec3f_t){{0.0f, 10.0f, 0.0f}});
    vec3f_copy(&light_data.camera_pos, &cam_pos);

    dusk_shader_file_t shader_files[] = {
        {GL_VERTEX_SHADER, "assets/default.vs.glsl"},
        {GL_FRAGMENT_SHADER, "assets/default.fs.glsl"},
        {0, NULL},
    };

    dusk_shader_init(&shader, shader_files);
    dusk_track_static_resource(DUSK_RSC_SHADER, &shader);
    light_data_index =
        dusk_shader_add_data(&shader, "LightData", &light_data, sizeof(light_data_t));

    model = dusk_load_model_from_file("assets/sponza/sponza.dmfz", &shader);

    dusk_run();
    dusk_term();

    return 0;
}
