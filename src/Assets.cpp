#include "Assets.hpp"
#include <cstring>

namespace GrumpyParrot
{
    Assets::Assets() : textureCount(0), splashTexture(nullptr), baseDirectory(nullptr) {}

    Assets::~Assets()
    {
        cleanup();
    }

    bool Assets::init(const char *baseDir)
    {
        textureCount = 0;
        baseDirectory = baseDir;
        Logger::debug("Initialized assets with base directory: %s", baseDir);
        return true;
    }

    int Assets::loadTexture(const char *relativePath)
    {
        // Check if texture is already loaded
        for (int i = 0; i < textureCount; i++)
        {
            if (strcmp(textures[i].path, relativePath) == 0)
            {
                if (textures[i].isLoaded)
                {
                    Logger::debug("Texture already loaded: %s", relativePath);
                    return i;
                }
            }
        }

        if (textureCount >= MAX_ASSETS)
        {
            Logger::error("Cannot load texture: maximum number of assets reached");
            return -1;
        }

        char fullPath[512];
        snprintf(fullPath, sizeof(fullPath), "%s%s", baseDirectory, relativePath);

        Texture2D texture = LoadTexture(fullPath);
        if (texture.id == 0)
        {
            Logger::error("Failed to load texture: %s", fullPath);
            return -1;
        }

        int index = textureCount++;
        textures[index].path = relativePath;
        textures[index].texture = texture;
        textures[index].isLoaded = true;

        Logger::info("Successfully loaded texture: %s", fullPath);
        return index;
    }

    Texture2D *Assets::getTexture(const char *relativePath)
    {
        for (int i = 0; i < textureCount; i++)
        {
            if (strcmp(textures[i].path, relativePath) == 0 && textures[i].isLoaded)
            {
                return &textures[i].texture;
            }
        }
        Logger::warning("Texture not found: %s", relativePath);
        return nullptr;
    }

    void Assets::unloadTexture(const char *relativePath)
    {
        for (int i = 0; i < textureCount; i++)
        {
            if (strcmp(textures[i].path, relativePath) == 0 && textures[i].isLoaded)
            {
                UnloadTexture(textures[i].texture);
                textures[i].isLoaded = false;

                // Clear shortcut if this is the texture
                if (strcmp(relativePath, TEXTURE_SPLASH) == 0)
                {
                    splashTexture = nullptr;
                }

                Logger::debug("Unloaded texture: %s", relativePath);
                return;
            }
        }
    }

    void Assets::preloadShortcuts()
    {
        Logger::info("Preloading texture shortcuts...");

        int splashIndex = loadTexture(TEXTURE_SPLASH);
        if (splashIndex >= 0)
        {
            splashTexture = &textures[splashIndex].texture;
            Logger::debug("Preloaded splash texture");
        }
    }

    void Assets::unloadShortcuts()
    {
        Logger::info("Unloading texture shortcuts...");

        if (splashTexture != nullptr)
        {
            unloadTexture(TEXTURE_SPLASH);
            splashTexture = nullptr;
        }
    }

    void Assets::cleanup()
    {
        unloadShortcuts();

        for (int i = 0; i < textureCount; i++)
        {
            if (textures[i].isLoaded)
            {
                UnloadTexture(textures[i].texture);
                textures[i].isLoaded = false;
            }
        }
        textureCount = 0;
        Logger::info("Cleaned up assets");
    }
}
