#shader vertex
#version 330 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

out vec3 ourNormal;
uniform mat4 MVP;

void main()
{
    gl_Position = MVP * position;
    ourNormal = normal;
}

#shader fragment
#version 330 core
out vec4 FragColor;
  
in vec3 ourNormal;

void main()
{
    //FragColor = texture(ourTexture, TexCoord);
    FragColor = vec4(ourNormal,1.0);
}