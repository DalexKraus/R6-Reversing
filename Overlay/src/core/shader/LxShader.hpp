#ifndef LX_SHADER_H
#define LX_SHADER_H

#include "lx.hpp"

//Only include the needed glm stuff
#include "glm/mat4x4.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

typedef GLuint* lxShader;
typedef GLuint lxShaderUniform;

lxShader    lxShaderCreate();
void        lxShaderCompile(lxShader shader, const char* shaderSource, GLuint shaderType);
void        lxShaderLink(lxShader shader, void (*bindAttributesCallback)());

// Attribute and uniform stuff
void                        lxShaderBindAttribute(lxShader shader, int attr, const char* variableName);
inline lxShaderUniform      lxShaderGetUniformLocation(lxShader shader, const char* name) { return glGetUniformLocation(*shader, name); }

using namespace glm;
inline void lxShaderUniformBool (lxShaderUniform uniform, const bool  value) { glUniform1i(uniform, value); }
inline void lxShaderUniformInt  (lxShaderUniform uniform, const int   value) { glUniform1i(uniform, value); }
inline void lxShaderUniformFloat(lxShaderUniform uniform, const float value) { glUniform1f(uniform, value); }
inline void lxShaderUniformVec2 (lxShaderUniform uniform, const vec2*  value) { glUniform2fv(uniform, 1, &(*value)[0]); }
inline void lxShaderUniformVec3 (lxShaderUniform uniform, const vec3*  value) { glUniform3fv(uniform, 1, &(*value)[0]); }
inline void lxShaderUniformVec4 (lxShaderUniform uniform, const vec4*  value) { glUniform4fv(uniform, 1, &(*value)[0]); }
inline void lxShaderUniformMat4 (lxShaderUniform uniform, const mat4*  value) { glUniformMatrix4fv(uniform, 1, GL_FALSE, &(*value)[0][0]); }

void        lxShaderDestroy(lxShader shader);
void        lxShaderStart(lxShader shader);
void        lxShaderStop();

#endif