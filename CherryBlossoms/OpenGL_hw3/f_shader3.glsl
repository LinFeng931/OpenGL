#version 330 core
in vec4 vColor;
out vec4 FragColor;

in vec2 vTexCoord;

uniform sampler2D uTexture;
uniform bool uHasTexture;

void main()
{
	vec4 texColor = uHasTexture ? texture(uTexture, vTexCoord) : vec4(1.0);
    FragColor = texColor * vColor;
}