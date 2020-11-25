#include "ShaderBase.hpp"

ShaderBase::ShaderBase(const char* vshFile, const char* fshFile, _SHDR_CALLBACK_FUNC(loadCustomUniforms))
{
    const char* vertexShaderSource      = readFileContent(vshFile);
    const char* fragmentShaderSource    = readFileContent(fshFile);

    m_shaderProgram = lxShaderCreate();
    lxShaderCompile(m_shaderProgram, vertexShaderSource, GL_VERTEX_SHADER);
    lxShaderCompile(m_shaderProgram, fragmentShaderSource, GL_FRAGMENT_SHADER);
    lxShaderLink(m_shaderProgram, NULL);

    free((void*) vertexShaderSource);
    free((void*) fragmentShaderSource);

    lxShaderStart(m_shaderProgram);
    //Load generic uniform locations
    if (loadCustomUniforms)
        loadUniformLocations();

    //Load individual (custom) uniforms
    loadCustomUniforms();

    lxShaderStop();
}

ShaderBase::~ShaderBase()
{
    lxShaderDestroy(m_shaderProgram);
}

void ShaderBase::loadUniformLocations()
{
    m_locationProjectionMatrix      = lxShaderGetUniformLocation(m_shaderProgram, "projectionMatrix");
    m_locationViewMatrix            = lxShaderGetUniformLocation(m_shaderProgram, "viewMatrix");
    m_locationTransformationMatrix  = lxShaderGetUniformLocation(m_shaderProgram, "transformationMatrix");
}

void ShaderBase::start()
{
    lxShaderStart(m_shaderProgram);
}

void ShaderBase::stop()
{
    lxShaderStop();
}
