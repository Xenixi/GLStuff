#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 vColor;
out vec4 colorFinal;
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    colorFinal = vec4(vColor.xyz , 1.0f);
}