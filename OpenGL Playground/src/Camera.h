#include "vendor/glm/gtc/matrix_transform.hpp"

class Camera 
{
private:
	glm::vec3 m_CamFront;
	glm::vec3 m_CamUp;
	bool m_FirstMouse;
	float m_LastX;
	float m_LastY;
	float m_Yaw;
	float m_Pitch;
	float m_Sensitivity;
	float m_Speed;
public:
	glm::vec3 m_CamPos;
	Camera();
	Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up,float speed);
	void Update(float xpos, float ypos);
	void Forward(float deltatime);
	void Backward(float deltatime);
	void Left(float deltatime);
	void Right(float deltatime);
	glm::mat4 ViewMat();
};