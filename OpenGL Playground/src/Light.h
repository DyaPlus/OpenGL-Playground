#include "vendor/glm/gtc/matrix_transform.hpp"

class Light
{
private:
	glm::vec3 m_Pos;
	glm::vec3 m_Color;
public:
	Light(glm::vec3 pos, glm::vec3 color);

};