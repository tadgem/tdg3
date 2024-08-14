#include "init.h"
#include "globals.h"
#include "imgui/imgui.h"
#include "sokol_app.h"
#include "sokol_fetch.h"
#include "sokol_fontstash.h"
#include "sokol_glue.h"
#include "sokol_gp.h"
#include "sokol_imgui.h"
#include "sokol_log.h"
#include <iostream>
#include "fontstash.h"


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
    fonsClearState(tdg::Globals::g_FontStashContext);

    float dpis = sapp_dpi_scale();
    sgl_defaults();
    sgl_matrix_mode_projection();
    sgl_ortho(0.0f, sapp_widthf(), sapp_heightf(), 0.0f, -1.0f, +1.0f);

    sx = 50*dpis; sy = 50*dpis;
    dx = sx; dy = sy;

    FONScontext* fs = tdg::Globals::g_FontStashContext;
    if (tdg::Globals::g_DefaultFont != FONS_INVALID) {
      fonsSetSize(fs, 18.0f*dpis);
      fonsSetFont(fs, tdg::Globals::g_DefaultFont);
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
    sgl_load_pipeline(tdg::Globals::g_DefaultPipeline);

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
    ImGui::ColorEdit3("Background", &tdg::Globals::g_DefaultPass.colors[0].clear_value.r, ImGuiColorEditFlags_None);
    if(ImGui::Button("Play Pedro!"))
    {
      std::cout << "TDG3 : Playing Pedro"<< std::endl;
      Mix_PlayChannel( -1, tdg::Globals::g_TestSample, 0 );
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

    pass.action = tdg::Globals::g_DefaultPass;
    pass.swapchain = sglue_swapchain();

    sg_begin_pass(&pass);
    sfons_flush(tdg::Globals::g_FontStashContext);
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
    if(ev->type == SAPP_EVENTTYPE_FOCUSED)
    {
      std::cout << "Gained Focus" << std::endl;
#ifdef __EMSCRIPTEN__
#endif
    }
}

sapp_desc sokol_main(int argc, char* argv[]) {
    sapp_desc app_desc{};
    app_desc.init_cb = tdg::engine_init::init;
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
