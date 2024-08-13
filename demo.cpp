#include "sokol_app.h"
#ifdef WIN32
#define SOKOL_D3D11
#elifdef __EMSCRIPTEN__
#define SOKOL_GLES3
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
    sgl_pipeline  depth_test_pip;
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
    sgl_pipeline_desc.depth.write_enabled = true;
    sgl_pipeline_desc.depth.compare = SG_COMPAREFUNC_LESS_EQUAL;

    // a pipeline object with less-equal depth-testing
    state.depth_test_pip = sgl_make_pipeline(sgl_pipeline_desc);

    auto sokol_imgui_description = simgui_desc_t{ };
    simgui_setup(&sokol_imgui_description);

    // initial clear color
    auto pass_action = sg_pass_action {};
    pass_action.colors[0] = { SG_LOADACTION_CLEAR, SG_STOREACTION_STORE,  { 0.0f, 0.5f, 1.0f, 1.0 } };
    state.pass_action = pass_action;
}

static void grid(float y, uint32_t frame_count) {
    const int num = 64;
    const float dist = 4.0f;
    const float z_offset = (dist / 8) * (frame_count & 7);
    sgl_begin_lines();
    for (int i = 0; i < num; i++) {
      float x = i * dist - num * dist * 0.5f;
      sgl_v3f(x, y, -num * dist);
      sgl_v3f(x, y, 0.0f);
    }
    for (int i = 0; i < num; i++) {
      float z = z_offset + i * dist - num * dist;
      sgl_v3f(-num * dist * 0.5f, y, z);
      sgl_v3f(num * dist * 0.5f, y, z);
    }
    sgl_end();
}


static void frame() {

    const float aspect = sapp_widthf() / sapp_heightf();
    static uint32_t frame_count = 0;
    frame_count > 60 ? frame_count = 0 : frame_count++;

    sgl_defaults();
    sgl_push_pipeline();
    sgl_load_pipeline(state.depth_test_pip);
    sgl_matrix_mode_projection();
    sgl_perspective(sgl_rad(45.0f), aspect, 0.1f, 1000.0f);
    sgl_matrix_mode_modelview();
    sgl_translate(sinf(frame_count * 0.02f) * 16.0f, sinf(frame_count * 0.01f) * 4.0f, 0.0f);
    sgl_c3f(1.0f, 0.0f, 1.0f);
    grid(-7.0f, frame_count);

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
    app_desc.window_title = "Hello Sokol + Dear ImGui";
    app_desc.width = 800;
    app_desc.height = 600;
    app_desc.icon.sokol_default = true;
    app_desc.logger.func = slog_func;
    return app_desc;
}
