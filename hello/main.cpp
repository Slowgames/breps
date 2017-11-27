
#include <stdexcept>
#include <memory>

#include <bx/os.h>

#include <SDL.h>
BX_PRAGMA_DIAGNOSTIC_PUSH()
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG("-Wextern-c-compat")
#include <SDL_syswm.h>
BX_PRAGMA_DIAGNOSTIC_POP()

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#if defined(None) // X11 defines this...
#	undef None
#endif // defined(None)

namespace sdl2 {


using window_ptr_t = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;
window_ptr_t make_window(uint32_t width, uint32_t height)
{
  return window_ptr_t(
    SDL_CreateWindow("hello", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL),
    SDL_DestroyWindow
    );
}

}

int init_bgfx(bgfx::RendererType::Enum renderer_type, sdl2::window_ptr_t& window)
{
  int result = 0;

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
      platform_data.ndt = nullptr;
      platform_data.nwh = info.info.win.window;
    } break;
    default: {
      platform_data.ndt = nullptr;
      platform_data.nwh = nullptr;
    } break;
  }

  bgfx::setPlatformData(platform_data);

  if (!bgfx::init(bgfx::RendererType::OpenGL)) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to initialize bgfx!");
    result = 1;
  }
  else {
    bgfx::reset(width, height, BGFX_RESET_VSYNC);
    bgfx::setViewClear(0
      , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
      , 0x303030ff
      , 1.0f
      , 0
			);
  }

  return result;
}

int main(int argc, char* argv[])
{
  int result = 0;

  uint32_t width = 1280, height = 720;

  auto init_flags = SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER | SDL_INIT_VIDEO;
  result = SDL_Init(init_flags);
  if (result != 0) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to initialize SDL: %s", SDL_GetError());
  }
  else {
    auto window = sdl2::make_window(width, height);
    if (!window) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to create window! %s", SDL_GetError);
    }
    else {
      result = init_bgfx(bgfx::RendererType::OpenGL, window);
      if (result == 0) {
        SDL_Delay(3000);
        bgfx::shutdown();
      }
    }
    SDL_Quit();
  }


  return result;
}
