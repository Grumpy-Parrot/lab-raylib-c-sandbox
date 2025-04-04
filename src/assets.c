#include "assets.h"

Texture2D* ASSETS_SPLASH = NULL;

bool assets_init(Assets* assets, const char* baseDir) {
    assets->textureCount = 0;
    strncpy(assets->baseDirectory, baseDir, MAX_PATH_LENGTH - 1);
    assets->baseDirectory[MAX_PATH_LENGTH - 1] = '\0';
    log_debug("Initialized assets with base directory: %s", baseDir);

    if (assets->baseDirectory[strlen(assets->baseDirectory) - 1] != '/') {
        strncat(assets->baseDirectory, "/", MAX_PATH_LENGTH - strlen(assets->baseDirectory) - 1);
    }

    return true;
}

int assets_load_texture(Assets* assets, const char* relativePath) {
    for (int i = 0; i < assets->textureCount; i++) {
        if (strcmp(assets->textures[i].path, relativePath) == 0) {
            if (assets->textures[i].isLoaded) {
                log_debug("Texture already loaded: %s", relativePath);
                return i;
            }
        }
    }

    if (assets->textureCount >= MAX_ASSETS) {
        log_error("Cannot load texture: maximum number of assets reached");
        return -1;
    }

    char fullPath[MAX_PATH_LENGTH];
    snprintf(fullPath, sizeof(fullPath), "%s/%s", assets->baseDirectory, relativePath);

    Texture2D texture = LoadTexture(fullPath);
    if (texture.id == 0) {
        log_error("Failed to load texture: %s", fullPath);
        return -1;
    }

    int index = assets->textureCount++;
    strncpy(assets->textures[index].path, relativePath, MAX_PATH_LENGTH - 1);
    assets->textures[index].path[MAX_PATH_LENGTH - 1] = '\0';
    assets->textures[index].texture = texture;
    assets->textures[index].isLoaded = 1;

    log_info("Successfully loaded texture: %s", fullPath);
    return index;
}

Texture2D* assets_get_texture(Assets* assets, const char* relativePath) {
    for (int i = 0; i < assets->textureCount; i++) {
        if (strcmp(assets->textures[i].path, relativePath) == 0 && assets->textures[i].isLoaded) {
            return &assets->textures[i].texture;
        }
    }
    log_warning("Texture not found: %s", relativePath);
    return NULL;
}

void assets_unload_texture(Assets* assets, const char* relativePath) {
    for (int i = 0; i < assets->textureCount; i++) {
        if (strcmp(assets->textures[i].path, relativePath) == 0 && assets->textures[i].isLoaded) {
            UnloadTexture(assets->textures[i].texture);
            assets->textures[i].isLoaded = 0;

            // Clear shortcut if this is the texture
            if (strcmp(relativePath, TEXTURE_SPLASH) == 0) {
                ASSETS_SPLASH = NULL;
            }

            log_debug("Unloaded texture: %s", relativePath);
            return;
        }
    }
}

void assets_preload_shortcuts(Assets* assets) {
    log_info("Preloading texture shortcuts...");

    int splashIndex = assets_load_texture(assets, TEXTURE_SPLASH);
    if (splashIndex >= 0) {
        ASSETS_SPLASH = &assets->textures[splashIndex].texture;
        log_debug("Preloaded splash texture");
    }
}

void assets_unload_shortcuts(Assets* assets) {
    log_info("Unloading texture shortcuts...");

    if (ASSETS_SPLASH != NULL) {
        assets_unload_texture(assets, TEXTURE_SPLASH);
        ASSETS_SPLASH = NULL;
    }
}

void assets_cleanup(Assets* assets) {
    assets_unload_shortcuts(assets);

    for (int i = 0; i < assets->textureCount; i++) {
        if (assets->textures[i].isLoaded) {
            UnloadTexture(assets->textures[i].texture);
            assets->textures[i].isLoaded = 0;
        }
    }
    assets->textureCount = 0;
    log_info("Cleaned up assets");
}
