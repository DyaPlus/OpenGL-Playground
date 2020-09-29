#shader vertex
#version 330 core
layout(location = 0) in vec4 position;
uniform mat4 MVP;

void main() {
    gl_Position = MVP * position;
};

#shader fragment
#version 330 core
out vec3 color;
void main() {
    color = vec3(1, 0, 0);
}