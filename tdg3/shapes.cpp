#include "shapes.h"
#include "sokol_gfx.h"
#include "sokol_gl.h"

void tdg::shapes::cube_shape() {
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

void tdg::shapes::line(float sx, float sy, float ex, float ey)
{
  sgl_begin_lines();
  sgl_c4b(255, 255, 0, 128);
  sgl_v2f(sx, sy);
  sgl_v2f(ex, ey);
  sgl_end();
}
void tdg::shapes::draw_cube(glm::vec3 pos, glm::vec3 euler, glm::vec3 scale) {
  sgl_matrix_mode_modelview();
  sgl_translate(pos.x, pos.y, pos.z);
  sgl_scale(scale.x, scale.y, scale.z);
  sgl_rotate(sgl_rad(euler.x), 1.0f, 0.0f, 0.0f);
  sgl_rotate(sgl_rad(euler.y), 0.0f, 1.0f, 0.0f);
  sgl_rotate(sgl_rad(euler.z), 0.0f, 0.0f, 1.0f);

  tdg::shapes::cube_shape();
}
