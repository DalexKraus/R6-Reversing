#ifndef LX_CAMERA_H
#define LX_CAMERA_H

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

#include <stdio.h>

class LxCamera
{
private:
    float m_fov, m_aspect;
    float m_nearZ, m_farZ;

    glm::vec3 m_position;
    glm::mat4 m_projection;
    glm::mat4 m_view;

    glm::vec3 m_front, m_up, m_right;

public:
    LxCamera(float fov, float aspect, float nearZ, float farZ)
        : m_fov(fov), m_aspect(aspect), m_nearZ(nearZ), m_farZ(farZ)
    {
        m_up        = glm::vec3(0, 1, 0);
        m_right     = glm::vec3(0, 0, 0);
        m_front     = glm::vec3(0, 0, 0);

        m_position.x = m_position.y = m_position.z = 0;
        
        updateProjection();
        updateView();
    }

    void setPosition(glm::vec3* position)
    {
        memcpy(&m_position[0], &position[0], sizeof(glm::vec3));
    }

    void setFront(const glm::vec3* front)
    {
        memcpy(&m_front[0], &front[0], sizeof(glm::vec3));
    }

    void            updateProjection();
    void            updateView();
    
    //TODO: Make accessors read only (const?)
    glm::vec3*      getPosition()       { return &m_position;       }
    glm::vec3*      getUp()             { return &m_up;             }
    glm::vec3*      getFront()          { return &m_front;          }
    glm::vec3*      getRight()          { return &m_right;          }

    glm::mat4*      getProjection()     { return &m_projection;     }
    glm::mat4*      getView()           { return &m_view;           }

};

#endif