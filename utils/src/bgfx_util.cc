

#include "bgfx_util.h"
#include "sdl2_util.h"

#include "easylogging++.h"

#include <bx/os.h>

BX_PRAGMA_DIAGNOSTIC_PUSH()
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG("-Wextern-c-compat")
#include <SDL_syswm.h>
BX_PRAGMA_DIAGNOSTIC_POP()

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#if defined(None) // X11 defines this...
#	undef None
#endif // defined(None)


namespace bgfx_util {

result_t init(bgfx::RendererType::Enum renderer_type, sdl2::window_ptr_t& window)
{
  LOG(INFO) << "Initializing bgfx";

  result_t result = result_t::OK;

  int width, height;
  SDL_GetWindowSize(window.get(), &width, &height);

  SDL_SysWMinfo info;
  SDL_VERSION(&info.version);
  SDL_GetWindowWMInfo(window.get(), &info);

  bgfx::PlatformData platform_data;
  platform_data.context = nullptr;
  platform_data.backBuffer = nullptr;
  platform_data.backBufferDS = nullptr;
  switch(info.subsystem) {
    case SDL_SYSWM_WINDOWS: {
      LOG(DEBUG) << "bgfx platform is windows.";
      platform_data.ndt = nullptr;
      platform_data.nwh = info.info.win.window;
    } break;
    default: {
      LOG(WARN) << "Unsupported platform! We're probably going to crash!";
      platform_data.ndt = nullptr;
      platform_data.nwh = nullptr;
    } break;
  }

  bgfx::setPlatformData(platform_data);

  if (!bgfx::init(renderer_type)) {
    LOG(ERROR) << "Failed to initialize bgfx!";
    result = result_t::ERR;
  }
  else {
    LOG(INFO) << "Apply screen/frame initial states";
    bgfx::reset(width, height, BGFX_RESET_VSYNC);
    bgfx::setViewClear(0
      , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
      , 0x303030ff
      , 1.0f
      , 0
			);
    bgfx::setViewRect(0, 0, 0, uint16_t(width), uint16_t(height));
  }

  return result;
}

} // namespace bgfx_util
