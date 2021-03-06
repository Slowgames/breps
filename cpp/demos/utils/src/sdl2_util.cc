
#include "sdl2_util.h"

#include "easylogging++.h"


namespace sdl2 {

window_ptr_t make_window(const char* title, uint32_t width, uint32_t height)
{
  LOG(INFO) << "Creating new window " << width << "x" << height;
  return window_ptr_t(
    SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL),
    SDL_DestroyWindow
    );
}

} // namespace sdl2
