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
    ourNormal = mat3(transpose(inverse(Model))) * normal;
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

    float constant;
    float linear;
    float quadratic;
};

#define NR_POINT_LIGHTS 4  

out vec4 FragColor;
  
in vec3 ourNormal;
in vec2 ourTexCoord; 
in vec3 FragPos; //After interpolation between vertices of the triangle 
in vec3 viewVector; //After interpolation between vertices of the triangle 

uniform Light light[NR_POINT_LIGHTS];
uniform Material material;
uniform int numberOfActiveLights;
void main()
{
    vec3 result = vec3(0,0,0);
    for(int i = 0; i < numberOfActiveLights; ++i) //TODO : numbers of active lights should be set by application
    {
        //Calculate light intensity
        float lightDistance = length(light[i].pos - FragPos);
        float attenuation = 1 /(light[i].constant + light[i].linear * lightDistance + light[i].quadratic * (lightDistance * lightDistance)); 
        vec3 lightColor = light[i].color * attenuation;

        //Calculate Ambient Component Using Only Light Color
        float ambientStrength = 0.08;
        vec3 ambientCoeff = ambientStrength * light[i].color; //Not affected by attenuation
    
        //Calculate Diffuse Comp
        vec3 lightVector = normalize(light[i].pos - FragPos);
        vec3 norm = normalize(ourNormal);
        vec3 diffuseCoeff = max(dot(lightVector,norm),0) * lightColor;
    
        //Calculate Specular Comp
        vec3 viewVector = normalize(viewVector);
        vec3 reflectedVector = -reflect(lightVector,norm);
        vec3 specularCoeff = lightColor * vec3(texture(material.specular,ourTexCoord)) * pow(max(dot(reflectedVector,viewVector),0),material.shininess);

        result += (diffuseCoeff + ambientCoeff ) * vec3(texture(material.diffuse,ourTexCoord)) + specularCoeff; 
    }
    FragColor = vec4(result, 1.0);
}