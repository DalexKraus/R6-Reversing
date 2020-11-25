#include "HeadBoxRenderer.hpp"

#include "lxCrystal/Globals.hpp"
#include "lxCrystal/Types.hpp"

#include <glm/mat3x3.hpp>
#include <glm/gtc/matrix_transform.hpp>

static float vertices[] = {
    0.25f, 0.25, 0.0f,  // top right
    0.25, 0.0, 0.0f,    // bottom right
    0.0, 0.0, 0.0f,     // bottom left
    0.0, 0.25, 0.0f     // top left
};

static unsigned int indices[] = {   
    0, 1, 3,
    1, 2, 3
};
static unsigned int indexCount = 6;

static glm::vec3   headBoxColor(0.0, 0.0, 0.0);
static float       headBoxOpacity;

HeadBoxRenderer::HeadBoxRenderer()
{
    m_headBoxShader = new HeadBoxShader();

    m_vao = lxVaoCreate();
    lxVaoStoreIndicesList(m_vao, indices, indexCount * sizeof(unsigned int), indexCount, false);
    lxVaoStoreData(m_vao, 0, vertices, sizeof(vertices), 3, false);
}

HeadBoxRenderer::~HeadBoxRenderer()
{
    lxVaoDestroy(m_vao);
    delete m_headBoxShader;
}

void HeadBoxRenderer::update(const double& deltaTime)
{
    headBoxOpacity = 1.0;
    headBoxColor.r = (float) sin(Globals::time);
    headBoxColor.g = 1.0f;
    headBoxColor.b = (float) cos(Globals::time);
}

void HeadBoxRenderer::draw(const glm::mat4* projectionMatrix)
{
    glm::mat4* viewMatrix = Globals::camera->getView();

    lxVaoBind(m_vao);
    m_headBoxShader->start();

    // Pass the color to the shader
    lxShaderUniformVec3(m_headBoxShader->m_locationHeadBoxColor, &headBoxColor);
    lxShaderUniformFloat(m_headBoxShader->m_locationHeadBoxOpacity, headBoxOpacity);

    //Enable wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnableVertexAttribArray(0);
    for (int i = 0; i < PLAYER_COUNT; i++)
    {
        Player_t currentPlayer = Globals::playerList[i];
        if (currentPlayer.health <= 0)
            continue;

        //Transform the quad onto the player's head
        // NOTE: Z and Y axis are swapped here (Rainbow uses Z axis as UP direction)
        glm::vec3 playerHead = glm::vec3(currentPlayer.headPos[0], currentPlayer.headPos[2], currentPlayer.headPos[1]);
        glm::mat4 transformationMatrix = glm::translate(glm::mat4(1), -playerHead);
        
        // Cancel out the rotation part of the view matrix
        glm::mat3 viewRotation  = glm::mat3(*viewMatrix);
        transformationMatrix   *= glm::mat4(glm::transpose(viewRotation));
        // Center the quad above the head
        transformationMatrix    = glm::translate(transformationMatrix, glm::vec3(-0.125f, -0.125f, 0.0f));

        lxShaderUniformMat4(m_headBoxShader->m_locationProjectionMatrix,        projectionMatrix);
        lxShaderUniformMat4(m_headBoxShader->m_locationViewMatrix,              viewMatrix);
        lxShaderUniformMat4(m_headBoxShader->m_locationTransformationMatrix,    &transformationMatrix);

        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    }

    //Disable wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    m_headBoxShader->stop();
}