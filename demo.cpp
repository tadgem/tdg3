#include "tdg3/globals.h"
#include "tdg3/init.h"
#include "tdg3/shapes.h"
#include "glm.hpp"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/cimport.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"
int mesh_vert_count = 0;
static void init()
{
  tdg::engine_init::init();
  Assimp::Importer importer;


  auto* scene = importer.ReadFile("../assets/models/Suzanne/glTF/Suzanne.gltf",
                                  aiProcess_Triangulate |
                                      aiProcess_CalcTangentSpace |
                                      aiProcess_OptimizeMeshes |
                                      aiProcess_GenSmoothNormals |
                                      aiProcess_OptimizeGraph |
                                      aiProcess_FixInfacingNormals |
                                      aiProcess_FindInvalidData |
                                      aiProcess_GenBoundingBoxes );
  mesh_vert_count = scene->mMeshes[0]->mNumVertices;

}

static void draw_some_text()
{
    // text rendering via fontstash.h
    float sx, sy, dx, dy, lh = 0.0f;
    uint32_t white = sfons_rgba(255, 255, 255, 255);
    uint32_t black = sfons_rgba(0, 0, 0, 255);
    uint32_t brown = sfons_rgba(192, 128, 0, 128);
    uint32_t blue  = sfons_rgba(0, 192, 255, 255);

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
      tdg::shapes::line(dx-10*dpis,dy,dx+250*dpis,dy);
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
      tdg::shapes::line(dx,dy-30*dpis,dx,dy+80.0f*dpis);
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
    tdg::shapes::draw_cube({0.0f, 0.0f, -12.0f}, {rot[0], rot[1], 0.f});
    sgl_rotate(sgl_rad(rot[0]), 1.0f, 0.0f, 0.0f);
    sgl_rotate(sgl_rad(rot[1]), 0.0f, 1.0f, 0.0f);
    tdg::shapes::cube_shape();
    sgl_push_matrix();
    sgl_translate(0.0f, 0.0f, 3.0f);
    sgl_scale(0.5f, 0.5f, 0.5f);
    sgl_rotate(-2.0f * sgl_rad(rot[0]), 1.0f, 0.0f, 0.0f);
    sgl_rotate(-2.0f * sgl_rad(rot[1]), 0.0f, 1.0f, 0.0f);
    tdg::shapes::cube_shape();
    sgl_push_matrix();
    sgl_translate(0.0f, 0.0f, 3.0f);
    sgl_scale(0.5f, 0.5f, 0.5f);
    sgl_rotate(-3.0f * sgl_rad(2*rot[0]), 1.0f, 0.0f, 0.0f);
    sgl_rotate(3.0f * sgl_rad(2*rot[1]), 0.0f, 0.0f, 1.0f);
    tdg::shapes::cube_shape();
    sgl_pop_matrix();
    sgl_pop_matrix();
}

static void frame() {

    glm::vec3 position {0};
    position += glm::vec3{1, 2, 3};
    // clear font state for frame
    fonsClearState(tdg::Globals::g_FontStashContext);
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


    ImGui::SetNextWindowPos({10, 10}, ImGuiCond_Once, {0,0 });
    ImGui::SetNextWindowSize({400, 400}, ImGuiCond_Once);
    ImGui::Begin("Sokol ImGui", 0, ImGuiWindowFlags_None);
    ImGui::ColorEdit3("Background", &tdg::Globals::g_DefaultPass.colors[0].clear_value.r, ImGuiColorEditFlags_None);
    ImGui::SliderFloat3("Sample vector", &position[0], - 100, 100);
    ImGui::InputInt("Model Num Vertices", &mesh_vert_count);
    if(ImGui::Button("Play Pedro!"))
    {
      std::cout << "TDG3 : Playing Pedro"<< std::endl;
      Mix_PlayChannel( -1, tdg::Globals::g_TestSample, 0 );
    }
    ImGui::End();

    // Draw an animated rectangle that rotates and changes its colors.
    float time = sapp_frame_count() * sapp_frame_duration();
    float r = sinf(time)*0.5+0.5, g = cosf(time)*0.5+0.5;
    sgp_set_color(r, g, 0.3f, 0.5f);
    sgp_rotate_at(time, 0.0f, 0.0f);
    sgp_draw_filled_rect(-0.1f, -0.1f, 1.0f, 1.0f);

    // end main loop
    auto pass = sg_pass {};

    pass.action = tdg::Globals::g_DefaultPass;
    pass.swapchain = sglue_swapchain();
    draw_some_text();

    sg_begin_pass(&pass);
    sgp_flush();
    sgp_end();
    sfons_flush(tdg::Globals::g_FontStashContext);
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
    if(ev->type == SAPP_EVENTTYPE_FOCUSED)
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
