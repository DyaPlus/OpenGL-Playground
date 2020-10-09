#shader vertex
#version 330 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

out vec3 ourNormal;
out vec3 FragPos;
out vec3 viewVector;
uniform mat4 MVP;
uniform mat4 Model;
uniform vec3 camPos;

void main()
{
    gl_Position = MVP * position;
    FragPos = vec3(Model * position); //This value should be interpolated between vertices in the fragment shader
    ourNormal = normal;
    viewVector = vec3(camPos- FragPos); //A vector in the direction of the visible vertices
}

#shader fragment
#version 330 core
out vec4 FragColor;
  
in vec3 ourNormal;
in vec3 FragPos; //After interpolation between vertices of the triangle 
in vec3 viewVector; //After interpolation between vertices of the triangle 

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
    
    vec3 viewVector = normalize(viewVector);
    vec3 reflectedVector = -reflect(lightVector,norm);
    //Specular coefficient doesnt use the object color , instead it used custom specular color as the second parameter and the light color as the first parameter
    vec3 specularCoeff = vec3(1.0,1.0,1.0) * vec3(0,0,1.0) * pow(max(dot(reflectedVector,viewVector),0),32);
    vec3 result = (diffuseCoeff + ambientCoeff ) * objectColor + specularCoeff; //A matte material always reflect the diffuse color (objectColor)
    FragColor = vec4(result, 1.0);
}