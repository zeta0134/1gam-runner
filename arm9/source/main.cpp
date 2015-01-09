#include <stdio.h>

#include <array>
#include <functional>

#include <nds.h>

#include "basic_mechanics.h"
#include "multipass_engine.h"
#include "debug.h"
#include "vram_allocator.h"

#include "ai/ship.h"

#include "game.h"

// Included to debug texture loading.

using ship_ai::ShipState;

using numeric_types::literals::operator"" _f;
using numeric_types::literals::operator"" _brad;
using numeric_types::fixed;

s32 const kTestPikmin{100};

MultipassEngine g_engine;
VramAllocator texture_allocator(VRAM_C, 128 * 1024);

//DrawableEntity g_ship;
//ShipState g_ship_state;
Game game(g_engine);

// Initialize the console using the full version of the console init function so
// that VRAM bank H can be used instead of the default bank, bank C.
void InitSubScreen() {
  vramSetBankH(VRAM_H_SUB_BG);
  videoSetModeSub(MODE_0_2D);

  PrintConsole* const kDefaultConsole{nullptr};
  s32 const kConsoleLayer{0};
  s32 const kConsoleMapBase{15};
  s32 const kConsoleTileBase{0};
  bool const kConsoleOnMainDisplay{true};
  bool const kLoadConsoleGraphics{true};
  consoleInit(kDefaultConsole, kConsoleLayer, BgType_Text4bpp, BgSize_T_256x256,
      kConsoleMapBase, kConsoleTileBase, not kConsoleOnMainDisplay,
      kLoadConsoleGraphics);
}

void InitDebugConsole() {
  consoleDebugInit(DebugDevice_NOCASH);
}

void InitMainScreen() {
  videoSetMode(MODE_0_3D);
  glInit();
  glEnable(GL_TEXTURE_2D | GL_BLEND);

  glClearColor(4, 4, 4, 31);
  // TODO(Nick?) Play with this - it may be why there used to be clipping at
  // the back plane.
  glClearDepth(0x7FFF);
  glViewport(0, 0, 255, 191);

  // Setup default lights; these will be overridden in the main engine.
  glLight(0, RGB15(31, 31, 31), floattov10(-0.40), floattov10(0.32),
      floattov10(0.27));
  glLight(1, RGB15(31, 31, 31), floattov10(0.32), floattov10(0.32),
      floattov10(0.32));
}

void LoadTextures() {
  // VRAM is not memory mapped to the CPU when in texture mode, so all
  // modifications to textures must be done by changing the bank to a mode
  // where it is mapped to the CPU, performing the modifications, and
  // switching it back to texture mode.
  vramSetBankC(VRAM_C_LCD);
  //texture_allocator.Load("piki_eyes", piki_eyes_img_bin, piki_eyes_img_bin_size);
  vramSetBankC(VRAM_C_TEXTURE);
}

void InitShip() {
  ShipState* state = game.SpawnObject<ShipState>();
  g_engine.TargetEntity(state->entity);
}

void Init() {
  InitSubScreen();
  InitDebugConsole();
  printf("Multipass Engine Demo\n");

  InitMainScreen();

  LoadTextures();
  InitShip();
  

  glPushMatrix();
}

void RunLogic() {
  //TODO: Make this more powerful, handle spawning objects and levels and stuff
  game.Step();
}

void GameLoop() {
  for (;;) {
    touchPosition touchXY;
    touchRead(&touchXY);

    //start debug timings for this loop
    debug::StartCpuTimer();
    debug::UpdateTopic();

    basicMechanicsUpdate();

    RunLogic();

    g_engine.Update();
    g_engine.Draw();
  }
}

int main() {
  Init();
  GameLoop();
  return 0;
}

