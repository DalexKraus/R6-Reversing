#ifndef TRACERS_RENDERER_H
#define TRACERS_RENDERER_H

#include "core/LxVao.hpp"
#include "lxcrystal/shader/ShaderBase.hpp"
#include "core/renderer/LxRenderer.hpp"

class TracersShader : public ShaderBase
{
private:
    void loadCustomUniforms()
    {
        m_locationTraceColor            = lxShaderGetUniformLocation(m_shaderProgram, "traceColor");
        m_locationTraceOpacity          = lxShaderGetUniformLocation(m_shaderProgram, "traceOpacity");
        m_locationTransformation_p1     = lxShaderGetUniformLocation(m_shaderProgram, "transformation_p1");
        m_locationTransformation_p2     = lxShaderGetUniformLocation(m_shaderProgram, "transformation_p2");
    }

public:
    lxShaderUniform m_locationTraceColor;
    lxShaderUniform m_locationTraceOpacity;
    lxShaderUniform m_locationTransformation_p1;
    lxShaderUniform m_locationTransformation_p2;

    TracersShader()
        : ShaderBase("res/shaders/tracers.vsh", "res/shaders/tracers.fsh", _SHDR_CALLBACK(&TracersShader::loadCustomUniforms))
    { }
};

class TracersRenderer : LxRenderer
{
private:
    lxVao           m_vao;
    TracersShader*  m_tracersShader;

public:
    TracersRenderer();
    ~TracersRenderer();

    void update (const double& deltaTime)           override;
    void draw   (const glm::mat4* projectionMatrix) override;
};

#endif