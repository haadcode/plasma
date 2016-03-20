// Based on:
// http://lodev.org/cgtutor/plasma.html
// https://gist.githubusercontent.com/stevenlr/824019/raw/094f1ffe38947eb364c650abb9086acbf906c00c/gistfile1.c

#include <SDL.h>
#include <stdio.h>

// Size of the window
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Sets the color of a pixel of 'screen' at x,y to r,g,b
void put_pixel(SDL_Surface *screen, int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
  unsigned long color = SDL_MapRGB(screen->format, r, g, b);
  unsigned long *buffer;

  buffer = (unsigned long *) screen->pixels + y * screen->pitch / 4 + x;
  *buffer = color;
}

// void create_palette(unsigned char* r, unsigned char* g, unsigned char* b)
void create_palette(SDL_Color* palette)
{
  for(int i = 0; i < 256; i ++)
  {
    // Try this
    // palette[i].r = 255 - ceil(128.0 + 128 * sin(3.1415 * i / 32.0));
    // palette[i].g = 128 - ceil(128.0 + 128 * sin(3.1415 * i / 64.0));
    // palette[i].b = 255 - ceil(128.0 + 128 * sin(3.1415 * i / 128.0));
    palette[i].r = 255 - ceil((sin(3.14 * 2 * i / 255) + 1) * 127);
    palette[i].g = ceil((sin(3.14 * 2 * i / 127.0) + 1) * 64);
    palette[i].b = 255 - palette[i].r;
  }
}

void render_plasma(SDL_Surface* screen, SDL_Color* palette, double time)
{
  for(int y = 0; y < SCREEN_HEIGHT; y++)
  {
    for(int x = 0; x < SCREEN_WIDTH; x++)
    {
      int color =
          (
              128.0 + (128.0 * sin(x / 16.0 + time))
            + 128.0 + (128.0 * sin(y / 8.0 + time))
            + 128.0 + (128.0 * sin((x + y) / 16.0))
            + 128.0 + (128.0 * sin(sqrt(ceil(x * x + y * y)) / 8.0))
          ) / 4;
      // Try this too
      // int color =
      //     (
      //         128.0 + (128.0 * sin(x / 16.0 + time))
      //       + 128.0 + (128.0 * sin(y / 32.0 + time))
      //       + 128.0 + (128.0 * sin(sqrt(ceil((x - SCREEN_WIDTH / 2.0)* (x - SCREEN_WIDTH / 2.0) + (y - SCREEN_HEIGHT / 2.0) * (y - SCREEN_HEIGHT / 2.0))) / 8.0))
      //       + 128.0 + (128.0 * sin(sqrt(ceil(x * x + y * y)) / 8.0))
      //     ) / 4;

      put_pixel(screen, x, y, palette[color].r, palette[color].g, palette[color].b);
    }
  }
}

int main( int argc, char* args[] )
{
  //The window we'll be rendering to
  SDL_Window* window = NULL;

  //The surface contained by the window
  SDL_Surface* screen = NULL;

  SDL_Event event;
  unsigned int previous_tick;

  // Palette buffer
  SDL_Color palette[256];

  int done = 1;
  double time = 0;

  if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("Can't initialize SDL! SDL_Error: %s\n", SDL_GetError());
  }
  else
  {
    //Create window
    window = SDL_CreateWindow("Plasma", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == NULL)
    {
      printf("Can't create the window! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
      // Get window surface
      screen = SDL_GetWindowSurface(window);

      create_palette(palette);

      previous_tick = SDL_GetTicks();
      while(done == 1)
      {
        while(SDL_PollEvent(&event))
        {
          if(event.type == SDL_QUIT)
            done = 0;
        }

        // Update the plasma
        render_plasma(screen, palette, time);

        // Update time
        time += 0.1;

        if(SDL_GetTicks() - previous_tick < 40)
        {
          SDL_Delay(40 - (SDL_GetTicks() - previous_tick));
          SDL_UpdateWindowSurface(window);
        }
        previous_tick = SDL_GetTicks();
      }
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
