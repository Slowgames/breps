
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

} // namespace sdl2

enum class result_t {
  OK, ERR
};

result_t init_bgfx(bgfx::RendererType::Enum renderer_type, sdl2::window_ptr_t& window)
{
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
    result = result_t::ERR;
  }
  else {
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

result_t update(float delta) {
  return result_t::OK;
}

result_t render() {
  // dummy draw call
  bgfx::touch(0);

  bgfx::frame();

  return result_t::OK;
}

result_t run() {
  result_t status = result_t::OK;

  uint32_t width = 1280, height = 720;
  auto window = sdl2::make_window(width, height);
  if (!window) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to create window! %s", SDL_GetError);
    status = result_t::ERR;
  }
  else {
    status = init_bgfx(bgfx::RendererType::OpenGL, window);
    if (status == result_t::OK) {
      bgfx::frame();

      bool exitRequested = false;
      while (!exitRequested && status != result_t::ERR) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
          switch(event.type) {
            case SDL_QUIT: {
              exitRequested = true;
            } break;
          }
        }

        status = update(0.0f);
        if (status == result_t::OK) {
          status = render();
        }
      }

      bgfx::shutdown();
    }
  }
  return result_t::OK;
}

int main(int argc, char* argv[])
{
  result_t result = result_t::OK;

  auto init_flags = SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER | SDL_INIT_VIDEO;
  if (SDL_Init(init_flags) != 0) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to initialize SDL: %s", SDL_GetError());
    result = result_t::ERR;
  }
  else {
    result = run();
    SDL_Quit();
  }


  return (int)result;
}
