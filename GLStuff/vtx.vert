#version 330 core
layout(location = 0) in vec3 aPos;
out vec4 colorFinal;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    colorFinal = vec4(abs(aPos.x), abs(aPos.y), 0.5, 1.0);
}