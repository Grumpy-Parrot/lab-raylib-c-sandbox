#include "rl.h"
#include "logger.h"
#include "assets.h"
#include "shader.h"
#include <stdio.h>
#include <stdlib.h>

#define CHECK_FATAL(condition, message) \
    do { \
        if (!(condition)) { \
            log_error(message); \
            CloseWindow(); \
            return -1; \
        } \
    } while (0)

int main(void)
{
    set_log_level(LOG_LEVEL_DEBUG);
    log_info("Initializing Grumpy Parrot application...");

    InitWindow(800, 600, "Grumpy Parrot Basic Window");
    log_debug("Window initialized: %dx%d", 800, 600);

    Assets assets;
    const char *executablePath = GetApplicationDirectory();

    bool assetsInitialized = assets_init(&assets, executablePath);
    CHECK_FATAL(assetsInitialized, "Failed to initialize assets");

    assets_preload_shortcuts(&assets);

    float scale = 0.5f;
    float scaledWidth = ASSETS_SPLASH->width * scale;
    float scaledHeight = ASSETS_SPLASH->height * scale;
    log_debug("Texture dimensions - Original: %dx%d, Scaled: %.0fx%.0f",
             ASSETS_SPLASH->width, ASSETS_SPLASH->height, scaledWidth, scaledHeight);

    Vector2 imagePosition = {
        (float)(GetScreenWidth() - scaledWidth) / 2,
        (float)(GetScreenHeight() - scaledHeight) / 2 - 20
    };

    log_info("Starting main application loop");

    Color backgroundColor = { 243, 230, 211, 255 };

    ShaderContext splashShader;
    bool splashShaderInitialized = shader_init(&splashShader, "splash", executablePath);
    CHECK_FATAL(splashShaderInitialized, "Failed to initialize shader");

    while (!WindowShouldClose())
    {
        shader_update(&splashShader);

        BeginDrawing();
            ClearBackground(backgroundColor);

            Rectangle source = { 0, 0, ASSETS_SPLASH->width, ASSETS_SPLASH->height };
            Rectangle dest = { imagePosition.x, imagePosition.y, scaledWidth, scaledHeight };
            shader_begin(&splashShader);
            DrawTexturePro(*ASSETS_SPLASH, source, dest, (Vector2){ 0, 0 }, 0.0f, WHITE);
            shader_end();

            DrawText("Welcome to Grumpy Parrot!",
                    GetScreenWidth()/2 - MeasureText("Welcome to Grumpy Parrot!", 20)/2,
                    (int)(imagePosition.y + scaledHeight + 20),
                    20,
                    BLACK);
        EndDrawing();
    }

    log_debug("Cleaning up resources...");
    shader_cleanup(&splashShader);
    assets_cleanup(&assets);
    CloseWindow();
    log_info("Application terminated successfully");

    return 0;
}
