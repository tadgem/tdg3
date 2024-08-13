#include "sokol_app.h"
#ifdef WIN32
#define SOKOL_D3D11
#else
#ifdef __EMSCRIPTEN__
#define SOKOL_GLES3
#endif
#endif

#include "sokol_gfx.h"
#include "sokol_log.h"
#include "sokol_glue.h"
#define SOKOL_GL_IMPL
#include "sokol_gl.h"
#include <stdio.h>  // needed by fontstash's IO functions even though they are not used
#define FONTSTASH_IMPLEMENTATION
#include "fontstash.h"
#define SOKOL_FONTSTASH_IMPL
#include "sokol_fontstash.h"
#include "imgui/imgui.h"
#include "sokol_imgui.h"
#include "sokol_gp.h"

static struct {
    sg_pass_action pass_action;
    sgl_pipeline    depth_test_pip;
} state;

static void init() {
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

    // a pipeline object with less-equal depth-testing
    state.depth_test_pip = sgl_make_pipeline(sgl_pipeline_desc);

    auto sokol_imgui_description = simgui_desc_t{ };
    simgui_setup(&sokol_imgui_description);

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

    const float aspect = sapp_widthf() / sapp_heightf();
    const float t = (float)(sapp_frame_duration() * 60.0);

    sgl_viewport(0, 0, sapp_width(), sapp_height(), true);
    //draw_triangle();
    draw_cubes(t);

    auto frame_desc = simgui_frame_desc_t{};
    frame_desc.width = sapp_width();
    frame_desc.height = sapp_height();
    frame_desc.delta_time = sapp_frame_duration();
    frame_desc.dpi_scale = sapp_dpi_scale();
    simgui_new_frame(&frame_desc);

    ImGui::SetNextWindowPos({10, 10}, ImGuiCond_Once, {0,0 });
    ImGui::SetNextWindowSize({400, 400}, ImGuiCond_Once);
    ImGui::Begin("Sokol ImGui", 0, ImGuiWindowFlags_None);
    ImGui::ColorEdit3("Background", &state.pass_action.colors[0].clear_value.r, ImGuiColorEditFlags_None);
    ImGui::End();

    /*=== UI CODE ENDS HERE ===*/

    auto pass = sg_pass {};

    pass.action = state.pass_action;
    pass.swapchain = sglue_swapchain();

    sg_begin_pass(&pass);
    sgl_draw();
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
