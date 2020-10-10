#shader vertex
#version 330 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 ourNormal;
out vec2 ourTexCoord;
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
    ourTexCoord = texCoord;
    viewVector = vec3(camPos - FragPos); //A vector in the direction of the visible vertices
}

#shader fragment
#version 330 core
/*struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};*/
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 pos;
    vec3 color;
};

out vec4 FragColor;
  
in vec3 ourNormal;
in vec2 ourTexCoord; 
in vec3 FragPos; //After interpolation between vertices of the triangle 
in vec3 viewVector; //After interpolation between vertices of the triangle 

uniform Light light;
uniform Material material;

void main()
{
    float ambientStrength = 0.08;
    vec3 ambientCoeff = ambientStrength * light.color;
    
    vec3 lightVector = normalize(light.pos - FragPos);
    vec3 norm = normalize(ourNormal);
    vec3 diffuseCoeff = max(dot(lightVector,norm),0) * light.color;
    
    vec3 viewVector = normalize(viewVector);
    vec3 reflectedVector = -reflect(lightVector,norm);
    //Specular coefficient doesnt use the object color , instead it used custom specular color as the second parameter and the light color as the first parameter
    vec3 specularCoeff = light.color * vec3(texture(material.specular,ourTexCoord)) * pow(max(dot(reflectedVector,viewVector),0),material.shininess);
    vec3 result = (diffuseCoeff + ambientCoeff ) * vec3(texture(material.specular,ourTexCoord)); //A matte material always reflect the diffuse color (objectColor)
    FragColor = vec4(result, 1.0);
}