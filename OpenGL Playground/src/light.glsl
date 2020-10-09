#shader vertex
#version 330 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * position;
}

#shader fragment
#version 330 core
out vec3 FragColor;
  
uniform vec3 color;

void main()
{
    FragColor = color;
}