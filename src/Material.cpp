#include "Material.hpp"

namespace GrumpyParrot
{
    Material::~Material()
    {
        cleanup();
    }

    bool Material::init(const std::string &name, const std::string &executablePath)
    {
        char vertexShaderPath[512];
        char fragmentShaderPath[512];
#ifdef PLATFORM_WASM
        std::snprintf(vertexShaderPath, sizeof(vertexShaderPath), "%s/shaders/glsl_es/%s.vertex.glsl", executablePath.c_str(), name.c_str());
        std::snprintf(fragmentShaderPath, sizeof(fragmentShaderPath), "%s/shaders/glsl_es/%s.fragment.glsl", executablePath.c_str(), name.c_str());
#else
        std::snprintf(vertexShaderPath, sizeof(vertexShaderPath), "%s/shaders/glsl/%s.vertex.glsl", executablePath.c_str(), name.c_str());
        std::snprintf(fragmentShaderPath, sizeof(fragmentShaderPath), "%s/shaders/glsl/%s.fragment.glsl", executablePath.c_str(), name.c_str());
#endif

        shader = LoadShader(vertexShaderPath, fragmentShaderPath);
        Logger::debug("Loaded shaders from: %s and %s", vertexShaderPath, fragmentShaderPath);

        shaderTimeRegister = GetShaderLocation(shader, "iTime");
        Logger::debug("Shader uniform 'iTime' location: %d", shaderTimeRegister);
        timeValue = 0.0f;

        return shaderTimeRegister != -1;
    }

    void Material::update()
    {
        timeValue += GetFrameTime();
        SetShaderValue(shader, shaderTimeRegister, &timeValue, SHADER_UNIFORM_FLOAT);
    }

    void Material::begin()
    {
        BeginShaderMode(shader);
    }

    void Material::end()
    {
        EndShaderMode();
    }

    void Material::cleanup()
    {
        if (shader.id > 0)
        {
            UnloadShader(shader);
            shader.id = 0;
        }
    }
}
