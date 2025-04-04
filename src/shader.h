#ifndef SHADER_H
#define SHADER_H

#include "rl.h"

typedef struct {
    Shader shader;
    int shaderTimeRegister;
    float timeValue;
} ShaderContext;

bool shader_init(ShaderContext* context, const char* name, const char* executablePath);
void shader_update(ShaderContext* context);
void shader_begin(ShaderContext* context);
void shader_end(void);
void shader_cleanup(ShaderContext* context);

#endif
