#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "nc1020.h"
#include <iostream>
#include <cstdint>
#include "col0.h"
#include "keys.h"

#define FRAME_RATE 30
#define FRAME_INTERVAL (1000/FRAME_RATE)

#define DT_SCREEN_WIDTH  1280
#define DT_SCREEN_HEIGHT 480

#define WQX_SCREEN_WIDTH 160
#define WQX_SCREEN_HEIGHT 80
#define LINE_SIZE 5

SDL_Renderer* renderer;
SDL_Texture *wqx_screen_texture;
SDL_Window* window;
SDL_Texture *indicator_num;
SDL_Texture *indicator_eng;
SDL_Texture *indicator_caps;
SDL_Texture *indicator_shift;
SDL_Texture *bottom_border;

static uint8_t lcd_buf[WQX_SCREEN_WIDTH * WQX_SCREEN_HEIGHT / 8];
static bool lcd_col0[WQX_SCREEN_HEIGHT];
static bool rctrl_down = false;

// was: RRGGBBAA (host is little endian)     AA   BB  GG  RR
//static const unsigned char on_color[4] = { 255, 0, 0, 0 };
//static const unsigned char off_color[4] = { 255, 60, 73, 61 };
//static const size_t color_size = sizeof(on_color)
constexpr uint32_t on_color = 0xff000000;
constexpr uint32_t off_color = 0xff717f70;
constexpr uint32_t off_color_r = 0x71;
constexpr uint32_t off_color_g = 0x7f;
constexpr uint32_t off_color_b = 0x70;

bool InitEverything() {
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    std::cout << " Failed to initialize SDL : " << SDL_GetError() << std::endl;
    return false;
  }
  window = SDL_CreateWindow("WQX", 0, 40, DT_SCREEN_WIDTH, DT_SCREEN_HEIGHT, 0);
  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
  if (!window) {
    std::cout << "Failed to create window : " << SDL_GetError() << std::endl;
    return false;
  }
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    std::cout << "Failed to create renderer : " << SDL_GetError() << std::endl;
    return false;
  }
  SDL_RenderSetLogicalSize(renderer, DT_SCREEN_WIDTH, DT_SCREEN_HEIGHT);

  wqx_screen_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
    SDL_TEXTUREACCESS_STREAMING, WQX_SCREEN_WIDTH, WQX_SCREEN_HEIGHT);

  SDL_Surface *psurface;

  psurface = IMG_Load("num.png");
  indicator_num = SDL_CreateTextureFromSurface(renderer, psurface);

  psurface = IMG_Load("eng.png");
  indicator_eng  = SDL_CreateTextureFromSurface(renderer, psurface);

  psurface = IMG_Load("shift.png");
  indicator_shift = SDL_CreateTextureFromSurface(renderer, psurface);

  psurface = IMG_Load("caps.png");
  indicator_caps = SDL_CreateTextureFromSurface(renderer, psurface);

  psurface = IMG_Load("bottom_border.png");
  bottom_border = SDL_CreateTextureFromSurface(renderer, psurface);

  wqx::WqxRom rom = {
    .romPath = "./obj_lu.bin",
    .norFlashPath = "./nc1020.fls",
    .statesPath = "./nc1020.sts",
  };
  wqx::Initialize(rom);
  wqx::LoadNC1020();
  
  return true;
}

void draw_digit(SDL_Renderer* renderer, int x, int y, const uint8_t* segs)
{
  SDL_Rect seg0 = { x, y, 2, 10 };
  SDL_Rect seg1 = { x, y, 10, 2 };
  SDL_Rect seg2 = { x + 10, y, 2, 10 };
  SDL_Rect seg3 = { x, y + 10, 10, 2 };
  SDL_Rect seg4 = { x +10, y + 10, 2, 10 };
  SDL_Rect seg5 = { x, y + 20, 10, 2 };
  SDL_Rect seg6 = { x, y + 10, 2, 10 };

  if (lcd_col0[segs[0]]) {
    SDL_RenderFillRect(renderer, &seg0);
  }
  if (lcd_col0[segs[1]]) {
    SDL_RenderFillRect(renderer, &seg1);
  }
  if (lcd_col0[segs[2]]) {
    SDL_RenderFillRect(renderer, &seg2);
  }
  if (lcd_col0[segs[3]]) {
    SDL_RenderFillRect(renderer, &seg3);
  }
  if (lcd_col0[segs[4]]) {
    SDL_RenderFillRect(renderer, &seg4);
  }
  if (lcd_col0[segs[5]]) {
    SDL_RenderFillRect(renderer, &seg5);
  }
  if (lcd_col0[segs[6]]) {
    SDL_RenderFillRect(renderer, &seg6);
  }
}

void draw_indicator(SDL_Renderer* renderer, int x, int y, SDL_Texture *img, uint8_t idx)
{
  if (!lcd_col0[idx]) {
    return;
  }

  SDL_Rect src = { 0, 0, 160, 48 };
  SDL_Rect dst = { x, y, 80, 24 };

  SDL_RenderCopy(renderer, img, &src, &dst);
}

void draw_vbar_component(SDL_Renderer* renderer, int x, int y, int h)
{
  ++y;
  for (int i = 0; i < h; i += 5) {
    SDL_RenderDrawLine(renderer, x, y + i, x + 15, y + i);
  }
}

void draw_bars(SDL_Renderer* renderer, int x, int y, int w, int h)
{
  const int vbar_frame_h = h;
  const int hbar_frame_w = w;
  const int vbar_h = vbar_frame_h - 6;
  const int vbar_y = y + 3;
  const int vbar_comp_h = vbar_h / sizeof(lcd_vbar);

  const SDL_Rect vbar_frame = { x, y, 20, vbar_frame_h };

  if (lcd_col0[LCD_VBAR_FRAME]) {
    SDL_RenderDrawRect(renderer, &vbar_frame);
  }

  for(int i = 0; i < sizeof(lcd_vbar); ++i) {
    if (lcd_col0[lcd_vbar[i]]) {
      draw_vbar_component(renderer, x + 2, vbar_y + i * vbar_comp_h, vbar_comp_h);
    }
  }

}

void handle_key(SDL_Event* event) {
  bool key_down = event->type == SDL_KEYDOWN;
  switch ( event->key.keysym.sym ) {
    case SDLK_F10:
      wqx::SetKey(WQX_KEY_FMAIN, key_down);
      break;
    case SDLK_F9:
      wqx::SetKey(WQX_KEY_FGAME, key_down);
      break;
    case SDLK_F8:
      wqx::SetKey(WQX_KEY_FDATA, key_down);
      break;
    case SDLK_F5:
      wqx::SetKey(WQX_KEY_FDICT, key_down);
      break;
    case SDLK_F6:
      wqx::SetKey(WQX_KEY_FADDR, key_down);
      break;
    case SDLK_F7:
      wqx::SetKey(WQX_KEY_FCOMP, key_down);
      break;
    case SDLK_F11:
      wqx::SetKey(WQX_KEY_FNET , key_down);
      break;
    case SDLK_BACKQUOTE:
      wqx::SetKey(WQX_KEY_POWER, key_down);
      break;

    case SDLK_F1:
    case SDLK_INSERT:
    case SDLK_LEFTBRACKET:
      wqx::SetKey(WQX_KEY_F1   , key_down);
      break;
    case SDLK_F2:
    case SDLK_DELETE:
    case SDLK_BACKSPACE:
    case SDLK_RIGHTBRACKET:
      wqx::SetKey(WQX_KEY_F2   , key_down);
      break;
    case SDLK_F3:
    case SDLK_MINUS:
      wqx::SetKey(WQX_KEY_F3   , key_down);
      break;
    case SDLK_F4:
    case SDLK_EQUALS:
      wqx::SetKey(WQX_KEY_F4   , key_down);
      break;

    case SDLK_o:
      wqx::SetKey(WQX_KEY_O    , key_down);
      break;
    case SDLK_l:
      wqx::SetKey(WQX_KEY_L    , key_down);
      break;
    case SDLK_UP:
      wqx::SetKey(WQX_KEY_UP   , key_down);
      break;
    case SDLK_DOWN:
      wqx::SetKey(WQX_KEY_DOWN , key_down);
      break;
    case SDLK_p:
      wqx::SetKey(WQX_KEY_P    , key_down);
      break;
    case SDLK_RETURN:
      wqx::SetKey(WQX_KEY_ENTER, key_down);
      break;
    case SDLK_PAGEDOWN:
      wqx::SetKey(WQX_KEY_PGDN , key_down);
      break;
    case SDLK_RIGHT:
      wqx::SetKey(WQX_KEY_RIGHT, key_down);
      break;

    case SDLK_q:
      wqx::SetKey(WQX_KEY_Q    , key_down);
      break;
    case SDLK_w:
      wqx::SetKey(WQX_KEY_W    , key_down);
      break;
    case SDLK_e:
      wqx::SetKey(WQX_KEY_E    , key_down);
      break;
    case SDLK_r:
      wqx::SetKey(WQX_KEY_R    , key_down);
      break;
    case SDLK_t:
      wqx::SetKey(WQX_KEY_T    , key_down);
      break;
    case SDLK_y:
      wqx::SetKey(WQX_KEY_Y    , key_down);
      break;
    case SDLK_u:
      wqx::SetKey(WQX_KEY_U    , key_down);
      break;
    case SDLK_i:
      wqx::SetKey(WQX_KEY_I    , key_down);
      break;

    case SDLK_a:
      wqx::SetKey(WQX_KEY_A    , key_down);
      break;
    case SDLK_s:
      wqx::SetKey(WQX_KEY_S    , key_down);
      break;
    case SDLK_d:
      wqx::SetKey(WQX_KEY_D    , key_down);
      break;
    case SDLK_f:
      wqx::SetKey(WQX_KEY_F    , key_down);
      break;
    case SDLK_g:
      wqx::SetKey(WQX_KEY_G    , key_down);
      break;
    case SDLK_h:
      wqx::SetKey(WQX_KEY_H    , key_down);
      break;
    case SDLK_j:
      wqx::SetKey(WQX_KEY_J    , key_down);
      break;
    case SDLK_k:
      wqx::SetKey(WQX_KEY_K    , key_down);
      break;

    case SDLK_z:
      wqx::SetKey(WQX_KEY_Z    , key_down);
      break;
    case SDLK_x:
      wqx::SetKey(WQX_KEY_X    , key_down);
      break;
    case SDLK_c:
      wqx::SetKey(WQX_KEY_C    , key_down);
      break;
    case SDLK_v:
      wqx::SetKey(WQX_KEY_V    , key_down);
      break;
    case SDLK_b:
      wqx::SetKey(WQX_KEY_B    , key_down);
      break;
    case SDLK_n:
      wqx::SetKey(WQX_KEY_N    , key_down);
      break;
    case SDLK_m:
      wqx::SetKey(WQX_KEY_M    , key_down);
      break;
    case SDLK_PAGEUP:
      wqx::SetKey(WQX_KEY_PGUP , key_down);
      break;

    case SDLK_LCTRL:
      wqx::SetKey(WQX_KEY_HELP , key_down);
      break;
    case SDLK_LSHIFT:
      wqx::SetKey(WQX_KEY_SHIFT, key_down);
      break;
    case SDLK_TAB:
      wqx::SetKey(WQX_KEY_CAPS , key_down);
      break;
    case SDLK_ESCAPE:
      wqx::SetKey(WQX_KEY_ESC  , key_down);
      break;
    case SDLK_LALT:
      wqx::SetKey(WQX_KEY_SYM  , key_down);
      break;
    case SDLK_PERIOD:
    wqx::SetKey(WQX_KEY_DOT  , key_down);
    break;
    case SDLK_SPACE:
    wqx::SetKey(WQX_KEY_SPACE, key_down);
    break;
    case SDLK_LEFT:
    wqx::SetKey(WQX_KEY_LEFT , key_down);
    break;

    // extra mappings for numbers
    case SDLK_0:
      wqx::SetKey(WQX_KEY_SYM  , key_down);
      break;
    case SDLK_1:
      wqx::SetKey(WQX_KEY_B  , key_down);
      break;
    case SDLK_2:
      wqx::SetKey(WQX_KEY_N  , key_down);
      break;
    case SDLK_3:
      wqx::SetKey(WQX_KEY_M  , key_down);
      break;
    case SDLK_4:
      wqx::SetKey(WQX_KEY_G  , key_down);
      break;
    case SDLK_5:
      wqx::SetKey(WQX_KEY_H  , key_down);
      break;
    case SDLK_6:
      wqx::SetKey(WQX_KEY_J  , key_down);
      break;
    case SDLK_7:
      wqx::SetKey(WQX_KEY_T  , key_down);
      break;
    case SDLK_8:
      wqx::SetKey(WQX_KEY_Y  , key_down);
      break;
    case SDLK_9:
      wqx::SetKey(WQX_KEY_U  , key_down);
      break;

    default :  // unsupported
      break;
  }
}

void draw_lcdmatrix() 
{
  constexpr SDL_Rect bitblt_area = { 240, 40, WQX_SCREEN_WIDTH * LINE_SIZE, WQX_SCREEN_HEIGHT * LINE_SIZE };
  uint32_t *imgbuf = nullptr;
  int pitch = 0;
  static const SDL_Rect source = { 0, 0, WQX_SCREEN_WIDTH, WQX_SCREEN_HEIGHT };
  SDL_LockTexture(wqx_screen_texture, &source, reinterpret_cast<void**>(&imgbuf), &pitch);

  for (int i = 0; i < sizeof(lcd_buf); ++i) {
    for (int j = 0; j < 8; ++j) {
      bool pixel = (lcd_buf[i] & (1 << (7 - j))) != 0;
      imgbuf[i*8+j] = pixel ? on_color : off_color;
    }
  }

  for (int i = 0; i < WQX_SCREEN_HEIGHT; ++i) {
    lcd_col0[i] = imgbuf[i * WQX_SCREEN_WIDTH] == on_color;
    imgbuf[i * WQX_SCREEN_WIDTH] = off_color;
  }

  SDL_UnlockTexture(wqx_screen_texture);
  SDL_RenderCopy(renderer, wqx_screen_texture, &source, &bitblt_area);
}

void Render() {
  constexpr SDL_Rect viewport_area = { 0, 0, 1280, 480 };
  SDL_SetRenderDrawColor(renderer, off_color_r, off_color_g, off_color_b, 0xff);
  SDL_RenderFillRect(renderer, &viewport_area);
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
  draw_lcdmatrix();
  draw_digit(renderer, 180, 6, lcd_digit0);
  draw_digit(renderer, 194, 6, lcd_digit1);
  draw_digit(renderer, 208, 6, lcd_digit2);
  draw_digit(renderer, 222, 6, lcd_digit3);

  draw_indicator(renderer, 160, 40, indicator_num, LCD_NUM);
  draw_indicator(renderer, 160, 66, indicator_eng, LCD_ENG);
  draw_indicator(renderer, 160, 92, indicator_caps, LCD_CAPS);
  draw_indicator(renderer, 160, 118, indicator_shift, LCD_SHIFT);

  draw_bars(renderer, 120, 30, 100, 400);

  constexpr SDL_Rect bottom_border_area = { 0, 450, 1280, 30 };
  SDL_RenderCopy(renderer, bottom_border, NULL, &bottom_border_area);

  SDL_RenderPresent(renderer);
}

void RunGame() {
  bool loop = true;

  while (loop) {
    uint32_t tick = SDL_GetTicks();

    wqx::RunTimeSlice(FRAME_INTERVAL, false);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if ( event.type == SDL_QUIT ) {
        loop = false;
      } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
        handle_key(&event);
      }
    }
    if (!wqx::CopyLcdBuffer(lcd_buf)) {
      std::cout << "Failed to copy buffer renderer." << std::endl;
    }
    Render();
    tick = SDL_GetTicks() - tick;
    SDL_Delay(FRAME_INTERVAL < tick ? 0 : FRAME_INTERVAL - tick);
  }
}

int main(int argc, char* args[]) {
  if (!InitEverything())
    return -1;
  
  RunGame();
  wqx::SaveNC1020();

  return 0;
}
