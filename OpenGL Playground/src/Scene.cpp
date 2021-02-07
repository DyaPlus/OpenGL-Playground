#include "Scene.h"

Scene* Scene::active_scene;

Scene::Scene()
{
    ShaderManager::Init();
    Bloom::Init();
    ScreenQuad::Init();
}

void Scene::AddModel(Model* model)
{
    //TODO : Very bad code -> -> -> 
    //The code defaults all materials to the first available shader
    //The first available shader is assumed to be the Blinn Phong as it is the current default of the application
    m_Models.push_back(model);
    for (int i = 0; i < model->m_MaterialsCreated.size(); i++)
    {
        model->m_MaterialsCreated[i]->SetShader(m_Shaders[0]);
    }
}

void Scene::AddCamera(Camera* camera)
{
    m_Cameras.push_back(camera);
    if (m_Cameras.size() == 1)
    {
        m_ActiveCamera = camera;
    }
}

void Scene::AddShader(Shader* shader)
{
    m_Shaders.push_back(shader);
}

//TODO : refactor to support more post processing

void Scene::Render(bool ppfx = false)
{
    if (ppfx)
    {
        Bloom::Setup();
        Shader* bloom_shader = Bloom::GetShader();
        for (auto model : m_Models)
        {
            UpdateShaderParametersinShader(model, bloom_shader);
            model->DrawWithShader(bloom_shader);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        Bloom::Apply();
        /*Bloom::BindRenderedTexture();
        ShaderManager::QuadShader->Bind();
        ScreenQuad::Render();
        ShaderManager::QuadShader->Unbind();*/
        ShaderManager::PostProcShader->Bind();
        ScreenQuad::Render();
        ShaderManager::PostProcShader->Unbind();

    }
    else
    {
        for (auto model : m_Models)
        {
            UpdateShaderParameters(model);
            model->Render();
        }
        UpdateGlobalShadersParameters();
        LightManager::Get()->RenderPointLights();
    }
}



void Scene::OnGuiUpdate()
{

}

void Scene::OnUpdate()
{
    
}


void Scene::UpdateShaderParameters(Model* model)
{
    for (int i = 0; i < model->GetNumMeshes(); ++i)
    {
        glm::mat4 Model = model->GetMesh(i).ModelMat();
        glm::mat4 View = m_ActiveCamera->ViewMat();
        glm::mat4 MVP = m_ActiveCamera->ProjectionMat() * View * Model;

        //Shader dependent code (TODO : Introduce a better abstraction)
        model->GetMesh(i).m_Material->m_ShaderToUse->SetMatrix4("MVP", MVP);
        if (model->GetMesh(i).m_Material->m_ShaderToUse->GetType() == ShaderType::Basic)
        {
            model->GetMesh(i).m_Material->m_ShaderToUse->SetMatrix4("Model", Model);
            model->GetMesh(i).m_Material->m_ShaderToUse->SetVector3("camPos", m_ActiveCamera->m_CamPos);
        }
    }
}
void Scene::UpdateShaderParametersinShader(Model* model,Shader* shader)
{
    if (shader->GetType() == ShaderType::Basic || shader->GetType() == ShaderType::Bloom)
    {
        for (int i = 0; i < model->GetNumMeshes(); ++i)
        {
            glm::mat4 Model = model->GetMesh(i).ModelMat();
            glm::mat4 View = m_ActiveCamera->ViewMat();
            glm::mat4 MVP = m_ActiveCamera->ProjectionMat() * View * Model;

            //Shader dependent code (TODO : Introduce a better abstraction)
            shader->SetMatrix4("MVP", MVP);
            shader->SetMatrix4("Model", Model);
            shader->SetVector3("camPos", m_ActiveCamera->m_CamPos);
        }
    }
    else if(shader->GetType() == ShaderType::Light)
    {
        //Shader dependent code (TODO : Introduce a better abstraction)
        shader->SetMatrix4("View", m_ActiveCamera->ViewMat());
        shader->SetMatrix4("Projection", m_ActiveCamera->ProjectionMat());   
    }
}

void Scene::UpdateGlobalShadersParameters()
{
    //Shader dependent code (TODO : Introduce a better abstraction)
    ShaderManager::LightShader->SetMatrix4("View", m_ActiveCamera->ViewMat());
    ShaderManager::LightShader->SetMatrix4("Projection", m_ActiveCamera->ProjectionMat());


}

void Scene::UpdateDeltatime(float deltaTime)
{
    m_DeltaTime = deltaTime;
}

void Scene::ToggleFPSMode(GLFWwindow* window)
{
    if (m_FPSMode)
    {
        m_FPSMode = false;
        //TODO : disable the camera properly by reseting last known mouse position
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(window, Scene::mouse_callback_nav_dispacth);
    }
    else
    {
        m_FPSMode = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, Scene::mouse_callback_dispacth);

    }
}

void Scene::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    m_ActiveCamera->Update(xpos, ypos);
}

void Scene::mouse_callback_dispacth(GLFWwindow* window, double xpos, double ypos)
{
    if (active_scene)
    {
        active_scene->mouse_callback(window, xpos, ypos);
    }
}

void Scene::mouse_callback_nav_dispacth(GLFWwindow* window, double xpos, double ypos)
{
}

