#version 330 core
in vec2 textureCoordinates;
out vec4 FragColor;

uniform sampler2D textureSampler;

void main()
{
      FragColor = texture(textureSampler, textureCoordinates);
}