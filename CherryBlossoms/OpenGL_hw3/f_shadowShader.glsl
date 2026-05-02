#version 330 core
void main() {
    // 什麼都不需要，只寫入深度
    gl_FragDepth = gl_FragCoord.z;
}
