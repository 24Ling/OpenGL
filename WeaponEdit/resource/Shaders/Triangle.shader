#shader vertex
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 color;

out vec4 u_Color;
void main()
{
    gl_Position = vec4(aPos, 1.0);
    u_Color = color;
}

#shader fragment
#version 460 core
out vec4 FragColor;
in vec4 u_Color;
void main()
{
    FragColor = u_Color;
}