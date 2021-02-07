#include "Bloom.h"

//Initialize static members
unsigned int Bloom::m_Render_FB = 0;
unsigned int Bloom::m_Render_CB[2] = { 0,0 };
unsigned int Bloom::m_WIP_FB[2] = { 0,0 };
unsigned int Bloom::m_WIP_CB[2] = { 0,0 };

void Bloom::Init()
{
    //Generate Framebuffers
    glGenFramebuffers(1, &m_Render_FB);

    //TODO : number of color attachements must be defined by a template for further expansion
    glGenTextures(2, m_Render_CB);

    glBindFramebuffer(GL_FRAMEBUFFER, m_Render_FB);
    // create and attach depth buffer (renderbuffer)
    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);    

    for (unsigned int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, m_Render_CB[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1024, 768, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // attach texture to framebuffer
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_Render_CB[i], 0
        );
    }
    
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
   
    
    glGenFramebuffers(2, m_WIP_FB);
    glGenTextures(2, m_WIP_CB);

    for (unsigned int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_WIP_FB[i]);
        glBindTexture(GL_TEXTURE_2D, m_WIP_CB[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1024, 768, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_WIP_CB[i], 0
        );
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0); //Return to on screen buffers

    ShaderManager::PostProcShader->SetInteger("renderedSceneTex",0);
    ShaderManager::PostProcShader->SetInteger("bloomTex", 1);
    
    ShaderManager::ExtractBrightnessShader->SetInteger("material.diffuse", 0);
    ShaderManager::ExtractBrightnessShader->SetInteger("material.specular", 1);
    ShaderManager::ExtractBrightnessShader->SetInteger("material.normal", 2);
}

void Bloom::Setup()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_Render_FB);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Bloom::Apply()
{
    bool horizontal = true, first_iteration = true;
    int amount = 6;
    ShaderManager::GaussianShader->Bind();
    glActiveTexture(GL_TEXTURE0);
    for (unsigned int i = 0; i < amount; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_WIP_FB[horizontal]);
        ShaderManager::GaussianShader->SetInteger("horizontal", horizontal);
        ShaderManager::GaussianShader->Bind();
        glBindTexture(
            GL_TEXTURE_2D, first_iteration ? m_Render_CB[1] : m_WIP_CB[!horizontal]
        );
        ScreenQuad::Render();
        horizontal = !horizontal;
        if (first_iteration)
            first_iteration = false;
    }
    ShaderManager::GaussianShader->Unbind();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_Render_CB[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_WIP_CB[0]);
    glActiveTexture(GL_TEXTURE0);
}

void Bloom::BindRenderedTexture()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_WIP_CB[0]);
}

Shader* Bloom::GetShader()
{
    return ShaderManager::ExtractBrightnessShader;
}
