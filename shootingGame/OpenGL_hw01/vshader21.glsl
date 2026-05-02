#version 330 core
layout (location = 0) in vec3 aPos;    // Pos
layout (location = 1) in vec3 aColor;  // Color
layout (location = 2) in vec3 aNormal; // Normal
layout (location = 3) in vec2 aTex;    // Texture Coordinates
out vec3 vColor;
uniform mat4 mxModel;
uniform mat4 mxView;
uniform mat4 mxProj;
void main()
{
    vColor = aColor;
    gl_Position = mxProj*mxView*mxModel*vec4(aPos, 1.0);
}