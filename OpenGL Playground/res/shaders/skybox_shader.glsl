#shader vertex
#version 330 core
layout (location = 0) in vec3 position;
out vec3 ourTexCoords;
uniform mat4 MVP;

void main()
{
    ourTexCoords = position;
    gl_Position = MVP * vec4(position,1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec3 ourTexCoords;

uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, ourTexCoords); //Sample CubeMap with a given position vector
}