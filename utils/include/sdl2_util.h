
#pragma once

#include <memory>
#include <SDL.h>

namespace sdl2 {

using window_ptr_t = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;
window_ptr_t make_window(const char* title, uint32_t width, uint32_t height);

} // namespace sdl2
