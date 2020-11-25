#ifndef SHADERBASE_H
#define SHADERBASE_H

#include "core/shader/LxShader.hpp"
#include "util/fileutil.h"

#include <functional>
#define _SHDR_CALLBACK_FUNC(callbackName) std::function<void(void)> callbackName
#define _SHDR_CALLBACK(function) std::bind(function, this)

/**
 * Defines the base implementation for a shader named T.
 */
class ShaderBase
{
private:
    void loadUniformLocations();

public:
    lxShader            m_shaderProgram;
    lxShaderUniform     m_locationProjectionMatrix;
    lxShaderUniform     m_locationViewMatrix;
    lxShaderUniform     m_locationTransformationMatrix;

public:
    ShaderBase(const char* vshFile, const char* fshFile, _SHDR_CALLBACK_FUNC(loadCustomUniforms));
    ~ShaderBase();

    void start();
    void stop();
};

#endif