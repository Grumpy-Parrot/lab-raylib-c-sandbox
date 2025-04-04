#include "shader.h"
#include "logger.h"
#include <stdio.h>

bool shader_init(ShaderContext* context, const char* name, const char* executablePath) {
    char vertexShaderPath[512];
    char fragmentShaderPath[512];
    sprintf(vertexShaderPath, "%s/shaders/%s.vertex.glsl", executablePath, name);
    sprintf(fragmentShaderPath, "%s/shaders/%s.fragment.glsl", executablePath, name);

    context->shader = LoadShader(vertexShaderPath, fragmentShaderPath);
    log_debug("Loaded shaders from: %s and %s", vertexShaderPath, fragmentShaderPath);

    context->shaderTimeRegister = GetShaderLocation(context->shader, "iTime");
    log_debug("Shader uniform 'iTime' location: %d", context->shaderTimeRegister);
    context->timeValue = 0.0f;

    return context->shaderTimeRegister != -1;
}

void shader_update(ShaderContext* context) {
    context->timeValue += GetFrameTime();
    SetShaderValue(context->shader, context->shaderTimeRegister, &context->timeValue, SHADER_UNIFORM_FLOAT);
}

void shader_begin(ShaderContext* context) {
    BeginShaderMode(context->shader);
}

void shader_end(void) {
    EndShaderMode();
}

void shader_cleanup(ShaderContext* context) {
    UnloadShader(context->shader);
}
