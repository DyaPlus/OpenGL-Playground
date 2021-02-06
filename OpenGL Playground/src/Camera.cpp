#include "Camera.h"

Camera::Camera()
	:m_CamPos(0,0,4),m_CamFront(0,0,-1),m_CamUp(0,1,0),m_FirstMouse(true),m_LastX(0),m_LastY(0)
	,m_Yaw(180),m_Pitch(0), m_Sensitivity(0.1f) , m_Speed(2.5f)
{
    m_Projection = glm::perspective(glm::radians(90.0f), (float)1024 / (float)768, 0.1f, 100.0f);
    //TODO : add function to set projection 
}
Camera::Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up,float speed)
	: m_CamPos(glm::normalize(pos)), m_CamFront(glm::normalize(front)), m_CamUp(glm::normalize(up))
	, m_FirstMouse(true), m_LastX(0), m_LastY(0) , m_Sensitivity(0.1f) , m_Speed(speed)
{
	front = glm::normalize(front);
    assert(front.x != 0 || front.z != 0);
	m_Pitch = glm::degrees(asin(front.y)); //TODO make sure the returned value is degree not radian
    m_Projection = glm::perspective(glm::radians(90.0f), (float)1024 / (float)768, 0.1f, 90.0f);

    if (front.x)
    {
        m_Yaw = glm::degrees(asin(front.x / cos(glm::radians(m_Pitch))));
    }
    else 
    {
        m_Yaw = glm::degrees(acos(front.z / cos(glm::radians(m_Pitch))));
    }
}

void Camera::Update(float xpos, float ypos)
{
    if (m_FirstMouse)
    {
        m_LastX = xpos;
        m_LastY = ypos;
        m_FirstMouse = false;
    }

    float xoffset = m_LastX - xpos;
    float yoffset = m_LastY - ypos;
    m_LastX = xpos;
    m_LastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    m_Yaw += xoffset;
    m_Pitch += yoffset;
    if (m_Pitch > 89.0f)
        m_Pitch = 89.0f;
    if (m_Pitch < -89.0f)
        m_Pitch = -89.0f;
    if (m_Yaw >= 360 || m_Yaw <= -360)
    {
        m_Yaw = 0;
    }

    glm::vec3 direction;
    direction.x = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    direction.y = sin(glm::radians(m_Pitch));
    direction.z = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_CamFront = glm::normalize(direction);
}

glm::mat4 Camera::ViewMat()
{
    return glm::lookAt(m_CamPos, m_CamPos + m_CamFront, m_CamUp);
}

glm::mat4 Camera::ProjectionMat()
{
	return m_Projection;
}

void Camera::Forward(float deltatime)
{
    m_CamPos += m_Speed * deltatime * m_CamFront;
}

void Camera::Backward(float deltatime)
{
    m_CamPos -= m_Speed * deltatime * m_CamFront;
}

void Camera::Left(float deltatime)
{
    m_CamPos -= glm::normalize(glm::cross(m_CamFront, m_CamUp)) * m_Speed * deltatime;
}

void Camera::Right(float deltatime)
{
    m_CamPos += glm::normalize(glm::cross(m_CamFront, m_CamUp)) * m_Speed * deltatime;
}

