
#pragma once

#include "types.h"
#include "sdl2_util.h"

#include <bgfx/bgfx.h>

namespace bgfx_util {

result_t init(bgfx::RendererType::Enum renderer_type, sdl2::window_ptr_t& window);

} // namespace gfx
