#version 330 core
layout(location = 0) in vec3 aPos;
out vec4 colorFinal;
uniform float adjusted;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    colorFinal = vec4(abs(aPos.x), abs(aPos.y), adjusted, 1.0f);
}