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
    sampler2D diffuse; //Sample unit is set in application code
    sampler2D specular;
    float shininess;
};

struct PointLight {
    vec3 pos;
    vec3 color;

    float constant;
    float linear;
    float quadratic;
};

struct DirLight {
    vec3 dir;
    vec3 color;
};

#define NR_POINT_LIGHTS 4  
#define NR_DIR_LIGHTS 1

out vec4 FragColor;
  
in vec3 ourNormal;
in vec2 ourTexCoord; 
in vec3 FragPos; //After interpolation between vertices of the triangle 
in vec3 viewVector; //After interpolation between vertices of the triangle 

uniform PointLight pointLight[NR_POINT_LIGHTS];
uniform DirLight dirLight[NR_DIR_LIGHTS];

uniform Material material;
uniform int numberOfActivePLights;
uniform int numberOfActiveDLights;

vec3 EvaluatePointLights()
{
    vec3 result = vec3(0,0,0);
    vec3 viewVector = normalize(viewVector);
    vec3 norm = normalize(ourNormal);

    for(int i = 0; i < numberOfActivePLights; ++i) //TODO : numbers of active lights should be set by application
    {
        //Calculate light intensity
        float lightDistance = length(pointLight[i].pos - FragPos);
        float attenuation = 1 /(pointLight[i].constant + pointLight[i].linear * lightDistance + pointLight[i].quadratic * (lightDistance * lightDistance)); 
        vec3 lightColor = pointLight[i].color * attenuation;

        //Calculate Diffuse Comp
        vec3 lightVector = normalize(pointLight[i].pos - FragPos);
        vec3 diffuseCoeff = max(dot(lightVector,norm),0) * lightColor;
    
        //Calculate Specular Comp Blinn-Phong
        vec3 halfvector = normalize(viewVector + lightVector);
        vec3 specularCoeff = lightColor * vec3(texture(material.specular,ourTexCoord)) * pow(max(dot(halfvector,norm),0),material.shininess);

        result += (diffuseCoeff ) * vec3(texture(material.diffuse,ourTexCoord)) + specularCoeff; 
    }

    return result;
}
vec3 EvaluateDirLights()
{
    vec3 result = vec3(0,0,0);
    vec3 viewVector = normalize(viewVector);
    vec3 norm = normalize(ourNormal);

    for(int i = 0; i < numberOfActiveDLights; ++i) //TODO : numbers of active lights should be set by application
    {
        //Calculate light intensity
        vec3 lightColor = dirLight[i].color;

        //Calculate Ambient Component Using Only Light Color
        float ambientStrength = 0.04;
        vec3 ambientCoeff = ambientStrength * dirLight[i].color; //Not affected by attenuation
    
        //Calculate Diffuse Comp
        vec3 lightVector = -normalize(dirLight[i].dir);
        vec3 diffuseCoeff = max(dot(lightVector,norm),0) * lightColor;
    
        //Calculate Specular Comp Blinn-Phong
        vec3 halfvector = normalize(viewVector + lightVector);
        vec3 specularCoeff = smoothstep(0.0f,1.0f,max(dot(lightVector,norm),0)) * lightColor * vec3(texture(material.specular,ourTexCoord)) * pow(max(dot(halfvector,norm),0),material.shininess); 

        result += (diffuseCoeff + ambientCoeff) * vec3(texture(material.diffuse,ourTexCoord)) + specularCoeff; 
    }

    return result;
}


void main()
{
    vec3 result = vec3(0,0,0);
    result += EvaluateDirLights() + EvaluatePointLights();
    FragColor = vec4(result, 1.0);
}