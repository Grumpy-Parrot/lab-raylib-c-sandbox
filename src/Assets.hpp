#pragma once

#include "RayLibAPI.h"
#include "Logger.hpp"
#include <string>
#include <array>
#include <memory>

namespace GrumpyParrot
{
    constexpr int MAX_ASSETS = 100;
    constexpr const char *TEXTURE_SPLASH = "resources/splash.png";

    class Assets
    {
    public:
        Assets();
        ~Assets();

        bool init(const char* baseDir);
        int loadTexture(const char* relativePath);
        Texture2D *getTexture(const char* relativePath);
        void unloadTexture(const char* relativePath);
        void cleanup();

        void preloadShortcuts();
        void unloadShortcuts();

        // Shortcuts for commonly used assets
        Texture2D *getSplashTexture() const { return splashTexture; }

    private:
        struct TextureAsset
        {
            const char* path;
            Texture2D texture;
            bool isLoaded = false;
        };

        std::array<TextureAsset, MAX_ASSETS> textures;
        int textureCount;
        const char* baseDirectory;

        // Shortcuts
        Texture2D *splashTexture;
    };
};
