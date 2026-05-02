#version 330 core
layout (location = 0) in vec3 aPos;    // Pos
layout (location = 1) in vec3 aColor;  // Color
layout (location = 2) in vec3 aNormal; // Normal
layout (location = 3) in vec2 aTex;    // Texture Coordinates

out vec4 vColor;
out vec2 vTexCoord;

uniform mat4 mxModel;
uniform mat4 mxView;
uniform mat4 mxProj;
uniform vec4 ui4Color;
uniform int  iColorType;

void main()
{
    if (iColorType == 1)
        vColor = vec4(aColor, 1.0); // vertex color
    else if (iColorType == 2)
        vColor = ui4Color; // object color
    else
        vColor = vec4(1.0); // fallback color (white), required!

    vTexCoord = aTex;
    gl_Position = mxProj * mxView * mxModel * vec4(aPos, 1.0);
}
