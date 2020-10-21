#shader vertex
#version 330 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

out vec3 viewVector;
out vec3 ourNormal;

uniform mat4 MVP;
uniform mat4 Model;

uniform vec3 camPos;

void main()
{
    ourNormal = mat3(transpose(inverse(Model))) * normal;
    vec3 FragPos = vec3(Model * position); //This value should be interpolated between vertices in the fragment shader
    viewVector = vec3(camPos - FragPos); //A vector in the direction of the visible vertices

    gl_Position = MVP * position;
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec3 viewVector;
in vec3 ourNormal;

uniform samplerCube skybox;

void main()
{
    vec3 reflectedVector = -reflect(viewVector,normalize(ourNormal));
    FragColor = texture(skybox, reflectedVector); //Sample CubeMap with a given position vector
}