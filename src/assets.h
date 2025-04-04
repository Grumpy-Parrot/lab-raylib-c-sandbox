#pragma once

#ifndef ASSETS_H
#define ASSETS_H

#include "rl.h"
#include "logger.h"
#include <stdio.h>
#include <string.h>

#define MAX_ASSETS 100
#define MAX_PATH_LENGTH 256

#define TEXTURE_SPLASH "resources/splash.png"

typedef struct {
    char path[MAX_PATH_LENGTH];
    Texture2D texture;
    int isLoaded;
} TextureAsset;

typedef struct {
    TextureAsset textures[MAX_ASSETS];
    int textureCount;
    char baseDirectory[MAX_PATH_LENGTH];
} Assets;

extern Texture2D* ASSETS_SPLASH;

bool assets_init(Assets* assets, const char* baseDir);
int assets_load_texture(Assets* assets, const char* relativePath);
Texture2D* assets_get_texture(Assets* assets, const char* relativePath);
void assets_unload_texture(Assets* assets, const char* relativePath);
void assets_cleanup(Assets* assets);

void assets_preload_shortcuts(Assets* assets);
void assets_unload_shortcuts(Assets* assets);

#endif
