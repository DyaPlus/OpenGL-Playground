#shader vertex
#version 330 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;
uniform mat4 MVP;

void main()
{
    gl_Position = MVP * position;
    ourColor = aColor;
    TexCoord = aTexCoord;
}

#shader fragment
#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    //FragColor = texture(ourTexture, TexCoord);
    FragColor = vec4(1.0,1.0,1.0,1.0);
}