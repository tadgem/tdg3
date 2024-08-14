#pragma once
#ifdef TDG_INIT_IMPL
#define SOKOL_IMPL
#ifdef WIN32
#define SOKOL_D3D11
#else
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define SOKOL_GLES3
#endif
#endif
#endif
#include "sokol_app.h"
#include "sokol_gfx.h"
#include <iostream>
#ifdef TDG_INIT_IMPL
#define SOKOL_GP_IMPL
#endif
#include "sokol_gp.h"
#include "sokol_log.h"
#include "sokol_glue.h"
#ifdef TDG_INIT_IMPL
#define SOKOL_GL_IMPL
#endif
#include "sokol_gl.h"
#ifdef TDG_INIT_IMPL
#define FONTSTASH_IMPLEMENTATION
#endif
#include "fontstash.h"
#ifdef TDG_INIT_IMPL
#define SOKOL_FONTSTASH_IMPL
#endif
#include "sokol_fontstash.h"
#include "imgui/imgui.h"
#include "sokol_imgui.h"
#ifdef TDG_INIT_IMPL
#define SOKOL_FETCH_IMPL
#endif
#include "SDL.h"
#include "SDL_mixer.h"
#include "sokol_fetch.h"

struct FONScontext;

namespace tdg
{
class Globals
{
public:
  /// Sokol gfx
  inline static sg_pass_action  g_DefaultPass;
  inline static sgl_pipeline    g_DefaultPipeline = {};

  // Fonts
  inline static FONScontext*    g_FontStashContext;
  inline static int             g_DefaultFont = -1;
  inline static uint8_t         g_FontDefaultData[256 * 1024] = {0};

  // Audio
  inline static Mix_Chunk*      g_TestSample;
};
}
