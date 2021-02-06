#include "GL/glew.h"
#include "Shader.h"
#include "ScreenQuad.h"

class Bloom
{
private:
	static unsigned int m_Render_FB;
	static unsigned int m_Render_CB[2];

	static unsigned int m_WIP_FB[2];
	static unsigned int m_WIP_CB[2];
public:
	Bloom() = default;
	static void Init();
	static void Setup();
	static void Apply();
	static void BindRenderedTexture();
	static Shader* GetShader();

};