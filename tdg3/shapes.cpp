#include "shapes.h"
#include "sokol_gfx.h"
#include "sokol_gl.h"

void tdg::shapes::cube() {
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