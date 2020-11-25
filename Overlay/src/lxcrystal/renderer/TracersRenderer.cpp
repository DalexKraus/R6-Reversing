#include "TracersRenderer.hpp"

#include "lxCrystal/Globals.hpp"
#include "lxCrystal/Types.hpp"

#include <glm/mat3x3.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Values are normalized so we can multiply by the enemie's position
// to end up exactly on the head.
static float vertices[] = {
    1.0, 1.0, 1.0,    // Center point
    1.0, 1.0, 1.0     // Point above enemy
};

static unsigned int indices[] = {
    0, 1
};

static unsigned int indexCount = 2;

static glm::vec3 traceColor = glm::vec3(1.0, 1.0, 0.0);
static float traceOpacity = 1.0f;

TracersRenderer::TracersRenderer()
{
    m_tracersShader = new TracersShader();
    m_vao = lxVaoCreate();
    lxVaoStoreIndicesList(m_vao, indices, indexCount * sizeof(unsigned int), indexCount, false);
    lxVaoStoreData(m_vao, 0, vertices, sizeof(vertices), 3, false);
}

TracersRenderer::~TracersRenderer()
{
    lxVaoDestroy(m_vao);
    delete m_tracersShader;
}

void TracersRenderer::update(const double& deltaTime)
{

}

void TracersRenderer::draw(const glm::mat4* projectionMatrix)
{
    glm::mat4* viewMatrix = Globals::camera->getView();
    Player_t* player = &Globals::playerList[Globals::playerIdx];
    glm::vec3 playerHead = glm::vec3(player->headPos[0], player->headPos[2], player->headPos[1]);
    glm::mat4 p1Transformation = glm::translate(glm::mat4(1), playerHead);  //Transformation for the fixed vertex (screen center)

    lxVaoBind(m_vao);
    m_tracersShader->start();

    // Pass color uniforms
    lxShaderUniformFloat(m_tracersShader->m_locationTraceOpacity, traceOpacity);
    lxShaderUniformVec3(m_tracersShader->m_locationTraceColor, &traceColor);
    lxShaderUniformMat4(m_tracersShader->m_locationTransformation_p1, &p1Transformation);

    glEnableVertexAttribArray(0);
    for (int i = 0; i < PLAYER_COUNT; i++)
    {
        Player_t currentPlayer = Globals::playerList[i];

        // Transform the line's ending onto the player's head by scaling
        // NOTE: Z and Y axis are swapped here (Rainbow uses Z axis as UP direction)
        glm::vec3 playerHead = glm::vec3(currentPlayer.headPos[0], currentPlayer.headPos[2], currentPlayer.headPos[1]);
        glm::mat4 transformationMatrix = glm::scale(glm::mat4(1), playerHead);

        lxShaderUniformMat4(m_tracersShader->m_locationProjectionMatrix,        projectionMatrix);
        lxShaderUniformMat4(m_tracersShader->m_locationViewMatrix,              viewMatrix);
         //Transformation for the target vertex
        lxShaderUniformMat4(m_tracersShader->m_locationTransformation_p2,       &transformationMatrix);

        glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, 0);
    }

    m_tracersShader->stop();
}