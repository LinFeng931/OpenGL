#version 330 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aColor;
layout(location=2) in vec3 aNormal;
layout(location=3) in vec2 aTex;
layout(location=4) in vec2 aLightMapTex;
layout(location=5) in vec3 aTangent;
layout(location=6) in vec3 aBitangent;

uniform mat4 mxModel;
uniform mat4 mxView;
uniform mat4 mxProj;
uniform vec3 viewPos;
uniform vec3 lightPos;

out vec3 vNormal;
out vec3 vLight;
out vec3 vView;
out vec3 vColor;
out vec3 v3Pos;
out vec2 vTexCoord;
out vec2 vLightMapUV;
out vec3 vTangent;
out vec3 vBitangent;

void main() {
    vec4 worldPos = mxModel * vec4(aPos, 1.0);
    v3Pos = worldPos.xyz;

    mat3 normalM = mat3(mxModel);
    vNormal = normalize(normalM * aNormal);
    vTangent = normalize(normalM * aTangent);
    vBitangent = normalize(normalM * aBitangent);

    vLight = normalize(lightPos - v3Pos);
    vView = normalize(viewPos - v3Pos);

    vColor = aColor;
    vTexCoord = aTex;
    vLightMapUV = aLightMapTex;

    gl_Position = mxProj * mxView * worldPos;
}
