#shader vertex
#version 330 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

out vec3 ourNormal;
out vec3 FragPos;
uniform mat4 MVP;
uniform mat4 Model;

void main()
{
    gl_Position = MVP * position;
    FragPos = vec3(Model * position); //This value should be interpolated between vertices in the fragment shader
    ourNormal = normal;
}

#shader fragment
#version 330 core
out vec4 FragColor;
  
in vec3 ourNormal;
in vec3 FragPos; //After interpolation between vertices of the triangle 

uniform vec3 lightPos;
uniform vec3 objectColor;

void main()
{
    vec3 ambientLightColor = vec3(1.0,1.0,1.0);
    float ambientStrength = 0.08;
    vec3 ambientCoeff = ambientStrength * ambientLightColor;
    
    vec3 lightVector = normalize(lightPos - FragPos);
    vec3 norm = normalize(ourNormal);
    vec3 diffuseCoeff = max(dot(lightVector,norm),0) * vec3(1.0,1.0,1.0);
    
    vec3 result = (diffuseCoeff + ambientCoeff) * objectColor; //A matte material always reflect the diffuse color (objectColor)
    FragColor = vec4(result, 1.0);
}