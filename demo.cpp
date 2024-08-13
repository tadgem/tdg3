//------------------------------------------------------------------------------
//  Simple C99 cimgui+sokol starter project for Win32, Linux and macOS.
//------------------------------------------------------------------------------
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_log.h"
#include "sokol_glue.h"
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "imgui/imgui.h"
#include "sokol_imgui.h"

static struct {
    sg_pass_action pass_action;
} state;

static void init() {
    auto sokol_gfx_description = sg_desc {};

    sokol_gfx_description.environment = sglue_environment();
    sokol_gfx_description.logger = {slog_func, nullptr};

    sg_setup(&sokol_gfx_description);
    auto sokol_imgui_description = simgui_desc_t{ };
    simgui_setup(&sokol_imgui_description);

    // initial clear color
    auto pass_action = sg_pass_action {};
    pass_action.colors[0] = { SG_LOADACTION_CLEAR, SG_STOREACTION_STORE,  { 0.0f, 0.5f, 1.0f, 1.0 } };
    state.pass_action = pass_action;
}

static void frame() {
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
