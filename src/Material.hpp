#pragma once

#include "RayLibAPI.h"
#include <string>
#include "Logger.hpp"
#include <cstdio>

namespace GrumpyParrot
{
    class Material
    {
    public:
        Material() = default;
        ~Material();

        bool init(const std::string &name, const std::string &executablePath);
        void update();
        void begin();
        static void end();
        void cleanup();

        Material(const Material &) = delete;
        Material &operator=(const Material &) = delete;

    private:
        ::Shader shader{0};
        int shaderTimeRegister = -1;
        float timeValue = 0.0f;
    };
};
