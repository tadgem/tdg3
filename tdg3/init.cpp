#include "init.h"
#define TDG_INIT_IMPL
#include "globals.h"

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
  tdg::Globals::g_TestSample = Mix_LoadWAV( "../assets/sounds/pedro.wav" );
  std::cout << "TDG3 : Pedro Audio Buffer Length : " << (float) (tdg::Globals::g_TestSample->alen / 1024) << " kB" << std::endl;
  Mix_VolumeChunk(tdg::Globals::g_TestSample, 255);
}

static void font_default_loaded(const sfetch_response_t* response) {
  if (response->fetched) {
    // TODO: create some kind of atlas
    // response->path
    tdg::Globals::g_DefaultFont = fonsAddFontMem(tdg::Globals::g_FontStashContext, "default", (unsigned char*)response->data.ptr, (int)response->data.size, false);
  }
}

void tdg::engine_init::init() {

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
  tdg::Globals::g_DefaultPipeline = sgl_make_pipeline(sgl_pipeline_desc);

  auto sokol_imgui_description = simgui_desc_t{ };
  simgui_setup(&sokol_imgui_description);

  const int atlas_dim = 512.0f;
  sfons_desc_t font_stash_desc = {};
  font_stash_desc.width = atlas_dim;
  font_stash_desc.height = atlas_dim;

  auto context = sfons_create(&font_stash_desc);
  tdg::Globals::g_FontStashContext = context;
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
  request.buffer = SFETCH_RANGE(tdg::Globals::g_FontDefaultData);
  sfetch_send(request);

  // initial clear color
  auto pass_action = sg_pass_action {};
  pass_action.colors[0] = { SG_LOADACTION_CLEAR, SG_STOREACTION_STORE,  { 0.0f, 0.5f, 1.0f, 1.0 } };
  pass_action.depth = {SG_LOADACTION_CLEAR, SG_STOREACTION_STORE, 1.0f};
  tdg::Globals::g_DefaultPass = pass_action;
}