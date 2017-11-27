
#include <SDL.h>


inline
void log_error(const char* message="%s")
{
  SDL_Log(message, SDL_GetError());
}

int main(int argc, char* argv[])
{
  int result = 0;

  auto init_flags = SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER | SDL_INIT_VIDEO;
  if (SDL_Init(init_flags) != 0)
  {
    log_error("Unable to initialize SDL: %s");
    result = 1;
  }
  else
  {
    //
  }

  return result;
}
