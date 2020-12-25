#include "Scene.h"

Scene* Scene::active_scene;

void Scene::AddModel(Model* model)
{
    m_Models.push_back(model);
}

void Scene::AddCamera(Camera* camera)
{
    m_Cameras.push_back(camera);
    if (m_Cameras.size() == 1)
    {
        m_ActiveCamera = camera;
    }
}

void Scene::Render()
{
    for (auto model : m_Models)
    {
        UpdateShaderParameters(model);
        model->Render();
    }
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
        model->GetMesh(i).m_ShaderToUse->SetMatrix4("MVP", MVP);
        if (model->GetMesh(i).m_ShaderToUse->GetType() == ShaderType::Basic)
        {
            model->GetMesh(i).m_ShaderToUse->SetMatrix4("Model", Model);
            model->GetMesh(i).m_ShaderToUse->SetVector3("camPos", m_ActiveCamera->m_CamPos);
        }
    }
}

void Scene::UpdateDeltatime(float deltaTime)
{
    m_DeltaTime = deltaTime;
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

