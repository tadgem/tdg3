#define SOKOL_IMPL
#ifdef WIN32
#define SOKOL_D3D11
#else
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define SOKOL_GLES3
#endif
#endif
#include "sokol_app.h"
#include "sokol_gfx.h"
#include <iostream>
#define SOKOL_GP_IMPL
#include "sokol_gp.h"
#include "sokol_log.h"
#include "sokol_glue.h"
#define SOKOL_GL_IMPL
#include "sokol_gl.h"
#define FONTSTASH_IMPLEMENTATION
#include "fontstash.h"
#define SOKOL_FONTSTASH_IMPL
#include "sokol_fontstash.h"
#include "imgui/imgui.h"
#include "sokol_imgui.h"
#define SOKOL_FETCH_IMPL
#include "sokol_fetch.h"
#include "SDL.h"
#include "SDL_mixer.h"


static struct {
    sg_pass_action  pass_action;
    sgl_pipeline    depth_test_pip;
    FONScontext*    fons_context;
    int             font_default = FONS_INVALID;
    uint8_t         font_default_data[256 * 1024];
    Mix_Chunk*      pedro_sample;
    bool            audio_initialized = false;
} state;

static void font_default_loaded(const sfetch_response_t* response) {
    if (response->fetched) {
      state.font_default = fonsAddFontMem(state.fons_context, "default", (unsigned char*)response->data.ptr, (int)response->data.size, false);
    }
}
static void init_sdl_mixer()
{
    std::cout << "TDG3 : Initializing SDL Audio" << std::endl;
    if(SDL_Init( SDL_INIT_AUDIO ) == -1 )
    {
      return;
    }

    std::cout << "TDG3 : Initializing SDL Mixer" << std::endl;
    std::cout << "TDG3 : Available Audio Devices" << std::endl;
    for(int i = 0; i < SDL_GetNumAudioDevices(0); i++)
    {
      std::cout << "    - " << SDL_GetAudioDeviceName(i, 0) << std::endl;
    }

    int chosen_device = Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) ;
    if(chosen_device == -1 )
    {
      return;
    }
    std::cout << "TDG3 : Selected Audio Device : " << SDL_GetAudioDeviceName(chosen_device, 0) << std::endl;

    std::cout << "TDG3 : Loading Pedro Audio" << std::endl;
    state.pedro_sample = Mix_LoadWAV( "../assets/sounds/pedro.wav" );
    std::cout << "TDG3 : Pedro Audio Buffer Length : " << (float) (state.pedro_sample->alen / 1024) << " kB" << std::endl;
    Mix_VolumeChunk(state.pedro_sample, 255);
}

static void init() {
    init_sdl_mixer();
    auto sokol_gfx_description = sg_desc {};

    sokol_gfx_description.environment = sglue_environment();
    sokol_gfx_description.logger = {slog_func, nullptr};

    sg_setup(&sokol_gfx_description);

    auto sgl_gfx_description = sgl_desc_t{};
    sgl_gfx_description.logger = {slog_func, nullptr};
    sgl_setup(sgl_gfx_description);

    auto sgl_pipeline_desc = sg_pipeline_desc{};
    sgl_pipeline_desc.cull_mode = SG_CULLMODE_BACK;
    sgl_pipeline_desc.depth.write_enabled = true;
    sgl_pipeline_desc.depth.compare = SG_COMPAREFUNC_LESS_EQUAL;
    state.depth_test_pip = sgl_make_pipeline(sgl_pipeline_desc);

    auto sokol_imgui_description = simgui_desc_t{ };
    simgui_setup(&sokol_imgui_description);

    const int atlas_dim = 512.0f;
    sfons_desc_t font_stash_desc = {};
    font_stash_desc.width = atlas_dim;
    font_stash_desc.height = atlas_dim;

    state.fons_context = sfons_create(&font_stash_desc);

    std::cout << "TDG3 : Initializing sokol-fetch" << std::endl;
    sfetch_desc_t fetch_setup_desc {};
    fetch_setup_desc.num_channels = 1;
    fetch_setup_desc.num_lanes = 4;
    fetch_setup_desc.logger.func = slog_func;
    sfetch_setup(&fetch_setup_desc);

    sgp_desc sgpdesc = {};
    sgp_setup(&sgpdesc);
    if (!sgp_is_valid()) {
      fprintf(stderr, "Failed to create Sokol GP context: %s\n", sgp_get_error_message(sgp_get_last_error()));
      exit(-1);
    }

    std::cout << "TDG3 : Requesting font asset" << std::endl;
    sfetch_request_t request {};
    request.path = "../assets/fonts/Zain-Black.ttf";
    request.callback = font_default_loaded;
    request.buffer = SFETCH_RANGE(state.font_default_data);
    sfetch_send(request);

    // initial clear color
    auto pass_action = sg_pass_action {};
    pass_action.colors[0] = { SG_LOADACTION_CLEAR, SG_STOREACTION_STORE,  { 0.0f, 0.5f, 1.0f, 1.0 } };
    pass_action.depth = {SG_LOADACTION_CLEAR, SG_STOREACTION_STORE, 1.0f};
    state.pass_action = pass_action;


}
static void cube(void) {
    sgl_begin_quads();
    sgl_c3f(1.0f, 0.0f, 0.0f);
    sgl_v3f_t2f(-1.0f,  1.0f, -1.0f, -1.0f,  1.0f);
    sgl_v3f_t2f( 1.0f,  1.0f, -1.0f,  1.0f,  1.0f);
    sgl_v3f_t2f( 1.0f, -1.0f, -1.0f,  1.0f, -1.0f);
    sgl_v3f_t2f(-1.0f, -1.0f, -1.0f, -1.0f, -1.0f);
    sgl_c3f(0.0f, 1.0f, 0.0f);
    sgl_v3f_t2f(-1.0f, -1.0f,  1.0f, -1.0f,  1.0f);
    sgl_v3f_t2f( 1.0f, -1.0f,  1.0f,  1.0f,  1.0f);
    sgl_v3f_t2f( 1.0f,  1.0f,  1.0f,  1.0f, -1.0f);
    sgl_v3f_t2f(-1.0f,  1.0f,  1.0f, -1.0f, -1.0f);
    sgl_c3f(0.0f, 0.0f, 1.0f);
    sgl_v3f_t2f(-1.0f, -1.0f,  1.0f, -1.0f,  1.0f);
    sgl_v3f_t2f(-1.0f,  1.0f,  1.0f,  1.0f,  1.0f);
    sgl_v3f_t2f(-1.0f,  1.0f, -1.0f,  1.0f, -1.0f);
    sgl_v3f_t2f(-1.0f, -1.0f, -1.0f, -1.0f, -1.0f);
    sgl_c3f(1.0f, 0.5f, 0.0f);
    sgl_v3f_t2f(1.0f, -1.0f,  1.0f, -1.0f,   1.0f);
    sgl_v3f_t2f(1.0f, -1.0f, -1.0f,  1.0f,   1.0f);
    sgl_v3f_t2f(1.0f,  1.0f, -1.0f,  1.0f,  -1.0f);
    sgl_v3f_t2f(1.0f,  1.0f,  1.0f, -1.0f,  -1.0f);
    sgl_c3f(0.0f, 0.5f, 1.0f);
    sgl_v3f_t2f( 1.0f, -1.0f, -1.0f, -1.0f,  1.0f);
    sgl_v3f_t2f( 1.0f, -1.0f,  1.0f,  1.0f,  1.0f);
    sgl_v3f_t2f(-1.0f, -1.0f,  1.0f,  1.0f, -1.0f);
    sgl_v3f_t2f(-1.0f, -1.0f, -1.0f, -1.0f, -1.0f);
    sgl_c3f(1.0f, 0.0f, 0.5f);
    sgl_v3f_t2f(-1.0f,  1.0f, -1.0f, -1.0f,  1.0f);
    sgl_v3f_t2f(-1.0f,  1.0f,  1.0f,  1.0f,  1.0f);
    sgl_v3f_t2f( 1.0f,  1.0f,  1.0f,  1.0f, -1.0f);
    sgl_v3f_t2f( 1.0f,  1.0f, -1.0f, -1.0f, -1.0f);
    sgl_end();
}

static void line(float sx, float sy, float ex, float ey)
{
    sgl_begin_lines();
    sgl_c4b(255, 255, 0, 128);
    sgl_v2f(sx, sy);
    sgl_v2f(ex, ey);
    sgl_end();
}

static void draw_some_text()
{
    // text rendering via fontstash.h
    float sx, sy, dx, dy, lh = 0.0f;
    uint32_t white = sfons_rgba(255, 255, 255, 255);
    uint32_t black = sfons_rgba(0, 0, 0, 255);
    uint32_t brown = sfons_rgba(192, 128, 0, 128);
    uint32_t blue  = sfons_rgba(0, 192, 255, 255);
    fonsClearState(state.fons_context);

    float dpis = sapp_dpi_scale();
    sgl_defaults();
    sgl_matrix_mode_projection();
    sgl_ortho(0.0f, sapp_widthf(), sapp_heightf(), 0.0f, -1.0f, +1.0f);

    sx = 50*dpis; sy = 50*dpis;
    dx = sx; dy = sy;

    FONScontext* fs = state.fons_context;
    if (state.font_default != FONS_INVALID) {
      fonsSetSize(fs, 18.0f*dpis);
      fonsSetFont(fs, state.font_default);
      fonsSetColor(fs, white);
      dx = 50*dpis; dy = 350*dpis;
      line(dx-10*dpis,dy,dx+250*dpis,dy);
      fonsSetAlign(fs, FONS_ALIGN_LEFT | FONS_ALIGN_TOP);
      dx = fonsDrawText(fs, dx,dy,"Top",NULL);
      dx += 10*dpis;
      fonsSetAlign(fs, FONS_ALIGN_LEFT | FONS_ALIGN_MIDDLE);
      dx = fonsDrawText(fs, dx,dy,"Middle",NULL);
      dx += 10*dpis;
      fonsSetAlign(fs, FONS_ALIGN_LEFT | FONS_ALIGN_BASELINE);
      dx = fonsDrawText(fs, dx,dy,"Baseline",NULL);
      dx += 10*dpis;
      fonsSetAlign(fs, FONS_ALIGN_LEFT | FONS_ALIGN_BOTTOM);
      fonsDrawText(fs, dx,dy,"Bottom",NULL);
      dx = 150*dpis; dy = 400*dpis;
      line(dx,dy-30*dpis,dx,dy+80.0f*dpis);
      fonsSetAlign(fs, FONS_ALIGN_LEFT | FONS_ALIGN_BASELINE);
      fonsDrawText(fs, dx,dy,"Left",NULL);
      dy += 30*dpis;
      fonsSetAlign(fs, FONS_ALIGN_CENTER | FONS_ALIGN_BASELINE);
      fonsDrawText(fs, dx,dy,"Center",NULL);
      dy += 30*dpis;
      fonsSetAlign(fs, FONS_ALIGN_RIGHT | FONS_ALIGN_BASELINE);
      fonsDrawText(fs, dx,dy,"Right",NULL);
    }
}

static void draw_cubes(const float t) {
    static float rot[2] = { 0.0f, 0.0f };
    rot[0] += 1.0f * t;
    rot[1] += 2.0f * t;

    sgl_defaults();
    sgl_load_pipeline(state.depth_test_pip);

    sgl_matrix_mode_projection();
    sgl_perspective(sgl_rad(45.0f), 1.0f, 0.1f, 100.0f);

    sgl_matrix_mode_modelview();
    sgl_translate(0.0f, 0.0f, -12.0f);
    sgl_rotate(sgl_rad(rot[0]), 1.0f, 0.0f, 0.0f);
    sgl_rotate(sgl_rad(rot[1]), 0.0f, 1.0f, 0.0f);
    cube();
    sgl_push_matrix();
    sgl_translate(0.0f, 0.0f, 3.0f);
    sgl_scale(0.5f, 0.5f, 0.5f);
    sgl_rotate(-2.0f * sgl_rad(rot[0]), 1.0f, 0.0f, 0.0f);
    sgl_rotate(-2.0f * sgl_rad(rot[1]), 0.0f, 1.0f, 0.0f);
    cube();
    sgl_push_matrix();
    sgl_translate(0.0f, 0.0f, 3.0f);
    sgl_scale(0.5f, 0.5f, 0.5f);
    sgl_rotate(-3.0f * sgl_rad(2*rot[0]), 1.0f, 0.0f, 0.0f);
    sgl_rotate(3.0f * sgl_rad(2*rot[1]), 0.0f, 0.0f, 1.0f);
    cube();
    sgl_pop_matrix();
    sgl_pop_matrix();
}

static void draw_triangle() {
    sgl_defaults();
    sgl_begin_triangles();
    sgl_v2f_c3b( 0.0f,  0.5f, 255, 0, 0);
    sgl_v2f_c3b(-0.5f, -0.5f, 0, 0, 255);
    sgl_v2f_c3b( 0.5f, -0.5f, 0, 255, 0);
    sgl_end();
}

static void frame() {

    // prep
    const float aspect = sapp_widthf() / sapp_heightf();
    int width = sapp_width(), height = sapp_height();
    const float t = (float)(sapp_frame_duration() * 60.0);
    sfetch_dowork();
    // Begin recording draw commands for a frame buffer of size (width, height).
    sgp_begin(width, height);
    // Set frame buffer drawing region to (0,0,width,height).
    sgp_viewport(0, 0, width, height);
    // Set drawing coordinate space to (left=-ratio, right=ratio, top=1, bottom=-1).
    sgp_project(-aspect, aspect, 1.0f, -1.0f);
    // Clear the frame buffer.
    sgl_viewport(0, 0, sapp_width(), sapp_height(), true);
    auto frame_desc = simgui_frame_desc_t{};
    frame_desc.width = sapp_width();
    frame_desc.height = sapp_height();
    frame_desc.delta_time = sapp_frame_duration();
    frame_desc.dpi_scale = sapp_dpi_scale();
    simgui_new_frame(&frame_desc);

    // Do main loop stuff
    //draw_triangle();
    draw_cubes(t);
    draw_some_text();


    ImGui::SetNextWindowPos({10, 10}, ImGuiCond_Once, {0,0 });
    ImGui::SetNextWindowSize({400, 400}, ImGuiCond_Once);
    ImGui::Begin("Sokol ImGui", 0, ImGuiWindowFlags_None);
    ImGui::ColorEdit3("Background", &state.pass_action.colors[0].clear_value.r, ImGuiColorEditFlags_None);
    if(ImGui::Button("Play Pedro!"))
    {
      std::cout << "TDG3 : Playing Pedro"<< std::endl;
      Mix_PlayChannel( -1, state.pedro_sample, 0 );
    }
    ImGui::End();

    // Draw an animated rectangle that rotates and changes its colors.
    float time = sapp_frame_count() * sapp_frame_duration();
    float r = sinf(time)*0.5+0.5, g = cosf(time)*0.5+0.5;
    sgp_set_color(r, g, 0.3f, 1.0f);
    sgp_rotate_at(time, 0.0f, 0.0f);
    sgp_draw_filled_rect(-0.5f, -0.5f, 1.0f, 1.0f);

    // end main loop
    auto pass = sg_pass {};

    pass.action = state.pass_action;
    pass.swapchain = sglue_swapchain();

    sg_begin_pass(&pass);
    sfons_flush(state.fons_context);
    sgl_draw();
    sgp_flush();
    sgp_end();
    simgui_render();
    sg_end_pass();
    sg_commit();
}

static void cleanup() {
    simgui_shutdown();
    sg_shutdown();
}

static void event(const sapp_event* ev) {
    simgui_handle_event(ev);
    if(ev->type == SAPP_EVENTTYPE_FOCUSED && !state.audio_initialized)
    {
      std::cout << "Gained Focus" << std::endl;
#ifdef __EMSCRIPTEN__
#endif
    }
}

sapp_desc sokol_main(int argc, char* argv[]) {
    sapp_desc app_desc{};
    app_desc.init_cb = init;
    app_desc.frame_cb = frame;
    app_desc.cleanup_cb = cleanup;
    app_desc.event_cb = event;
    app_desc.window_title = "tdg3";
    app_desc.width = 800;
    app_desc.height = 600;
    app_desc.icon.sokol_default = true;
    app_desc.logger.func = slog_func;
    return app_desc;
}
