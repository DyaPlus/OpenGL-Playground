#ifndef SCREENQUAD_H
#define SCREENQUAD_H
#include "Shader.h"

class ScreenQuad
{
private:
	static unsigned int m_QuadVAO;
public:
	ScreenQuad() = default;
	static void Init();
	static void Render();
};

#endif