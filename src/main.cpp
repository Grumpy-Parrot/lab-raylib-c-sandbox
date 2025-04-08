#include "Assets.hpp"
#include "Logger.hpp"
#include "Material.hpp"
#include "Platform.h"

GrumpyParrot::Assets *assets;
GrumpyParrot::Material *splashShader;
Vector2 imagePosition;
float scaledWidth, scaledHeight;
Color backgroundColor = {243, 230, 211, 255};

void on_frame() {
  splashShader->update();

  BeginDrawing();
  ClearBackground(backgroundColor);

  Texture2D *splashTexture = assets->getSplashTexture();
  Rectangle source = {0, 0, static_cast<float>(splashTexture->width),
                      static_cast<float>(splashTexture->height)};
  Rectangle dest = {imagePosition.x, imagePosition.y, scaledWidth,
                    scaledHeight};

  splashShader->begin();
  DrawTexturePro(*splashTexture, source, dest, (Vector2){0, 0}, 0.0f, WHITE);
  splashShader->end();

  DrawText("Welcome to Grumpy Parrot!",
           GetScreenWidth() / 2 -
               MeasureText("Welcome to Grumpy Parrot!", 20) / 2,
           (int)(imagePosition.y + scaledHeight + 20), 20, BLACK);

  EndDrawing();
}

void on_app_exit() {
  Logger::info("Exiting Grumpy Parrot application...");
  delete splashShader;
  splashShader = nullptr;

  delete assets;
  assets = nullptr;
}

int main(void) {
  Logger::setLevel(Logger::Level::Debug);
  Logger::info("Initializing Grumpy Parrot application...");

  InitWindow(800, 600, "Grumpy Parrot Basic Window");
  Logger::debug("Window initialized: %dx%d", 800, 600);

  const char *executablePath = GetApplicationDirectory();

  assets = new GrumpyParrot::Assets();
  bool assetsInitialized = assets->init(executablePath);
  CHECK_FATAL(assetsInitialized, "Failed to initialize assets");

  assets->preloadShortcuts();

  float scale = 0.5f;
  Texture2D *splashTexture = assets->getSplashTexture();
  scaledWidth = splashTexture->width * scale;
  scaledHeight = splashTexture->height * scale;
  imagePosition.x = (float)(GetScreenWidth() - scaledWidth) / 2;
  imagePosition.y = (float)(GetScreenHeight() - scaledHeight) / 2 - 20;

  splashShader = new GrumpyParrot::Material();
  bool splashShaderInitialized = splashShader->init("splash", executablePath);
  CHECK_FATAL(splashShaderInitialized, "Failed to initialize material");

#ifdef PLATFORM_WASM
  emscripten_set_main_loop(on_frame, 0, 1);
#else
  while (!WindowShouldClose()) {
    on_frame();
  }
#endif

#ifndef PLATFORM_WASM
  CloseWindow();
#endif

  on_app_exit();
  return 0;
}
