#ifndef HEAD_BOX_RENDERER_H
#define HEAD_BOX_RENDERER_H

#include "core/LxVao.hpp"
#include "lxcrystal/shader/ShaderBase.hpp"
#include "core/renderer/LxRenderer.hpp"

class HeadBoxShader : public ShaderBase
{
private:
    void loadCustomUniforms()
    {
        m_locationHeadBoxColor      = lxShaderGetUniformLocation(m_shaderProgram, "headBoxColor");
        m_locationHeadBoxOpacity    = lxShaderGetUniformLocation(m_shaderProgram, "headBoxOpacity"); 
    }

public:
    lxShaderUniform m_locationHeadBoxColor;
    lxShaderUniform m_locationHeadBoxOpacity;

    HeadBoxShader()
        : ShaderBase("res/shaders/headbox.vsh", "res/shaders/headbox.fsh", _SHDR_CALLBACK(&HeadBoxShader::loadCustomUniforms))
    { }
};

class HeadBoxRenderer : LxRenderer
{
private:
    lxVao           m_vao;  
    HeadBoxShader*  m_headBoxShader;

public:
    HeadBoxRenderer();
    ~HeadBoxRenderer();

    void update (const double& deltaTime)           override;
    void draw   (const glm::mat4* projectionMatrix) override;
};

#endif