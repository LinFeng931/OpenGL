#version 330 core
layout (location = 0) in vec3 aPos;      // 頂點位置
layout (location = 1) in vec3 aColor;    // 頂點顏色
out vec3 vColor;
void main()
{
    gl_Position = vec4(aPos, 1.0);
    vColor = aColor;
}