#version 330 core
#define MAX_LIGHTS 20

in vec3 vColor;
in vec3 vNormal;
in vec3 v3Pos;
in vec2 vTexCoord;
in vec3 vTangent;
in vec3 vBitangent;
in vec4 vFragPosLightSpace[MAX_LIGHTS];
in vec2 vLightMapUV;

uniform int uShadingMode;   // 1=頂點色, 2=物件色, 3=Per-Pixel  
uniform vec4 ui4Color;      // 模型顏色     
uniform vec3 viewPos;       // 鏡頭位置
uniform int uTextureFlags;  // 1: diffuse, 2: lightmap, 3: normal map, 4: cubemap

uniform sampler2D uDiffuseMap; // GL_TEXTURE0
uniform sampler2D uLightMap;   // GL_TEXTURE1
uniform sampler2D uNormalMap;  // GL_TEXTURE2
uniform samplerCube uEnvMap;   // GL_TEXTURE3

struct LightSource {
    vec3 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float constant;
    float linear;
    float quadratic;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    float exponent;
    int lightType;
};
uniform LightSource uLight[MAX_LIGHTS];

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};
uniform Material uMaterial;
uniform int uLightCount;

out vec4 FragColor;

void main() {
    if (uShadingMode == 1) {
        FragColor = vec4(vColor, 1.0);
        return;
    }
    if (uShadingMode == 2) {
        FragColor = ui4Color;
        return;
    }

    vec3 N = normalize(vNormal);
    vec3 V = normalize(viewPos - v3Pos);
    vec4 finalColor = vec4(0.0);

    vec3 colorRGB = uMaterial.diffuse.rgb;
    float alpha = uMaterial.diffuse.a;

    if ((uTextureFlags & 1) != 0) {
        vec4 diffTex = texture(uDiffuseMap, vTexCoord);
        colorRGB *= diffTex.rgb;
        alpha *= diffTex.a;
    }

    if ((uTextureFlags & 4) != 0) {
        vec3 T = normalize(vTangent);
        vec3 B = normalize(vBitangent);
        vec3 N0 = normalize(vNormal);
        mat3 TBN = mat3(T, B, N0);
        vec3 nmap = texture(uNormalMap, vTexCoord).rgb * 2.0 - 1.0;
        N = normalize(TBN * nmap);
    }

    if (alpha < 0.1)
        discard;

    for (int i = 0; i < uLightCount; ++i) {
        vec3 L = normalize(uLight[i].position - v3Pos);
        vec3 R = reflect(-L, N);

        vec4 ambient = uLight[i].ambient * uMaterial.ambient;
        float diff = max(dot(N, L), 0.0);
        float d = floor(diff * 3.0) / 3.0;
        vec4 diffuse = uLight[i].diffuse * d * vec4(colorRGB, 1.0);
        float specAngle = max(dot(R, V), 0.0);
        float spec = specAngle > 0.9 ? 1.0 : 0.0;
        vec4 specular = uLight[i].specular * spec * uMaterial.specular;

        float dist = length(uLight[i].position - v3Pos);
        float atten = 1.0 / (
            uLight[i].constant +
            uLight[i].linear * dist +
            uLight[i].quadratic * dist * dist
        );

        if (uLight[i].cutOff > 0.0) {
            float theta = dot(L, normalize(-uLight[i].direction));
            float intensity = clamp(
                (theta - uLight[i].outerCutOff) /
                (uLight[i].cutOff - uLight[i].outerCutOff),
                0.0, 1.0
            );
            atten *= pow(intensity, uLight[i].exponent);
        }

        finalColor += ambient * atten + (diffuse + specular) * atten;
    }

    /*if ((uTextureFlags & 2) != 0) {
        vec3 lightTex = texture(uLightMap, ).rgb;
        float lightMapStrength = 0.8;
        finalColor.rgb += lightTex * lightMapStrength;
    }*/
    // Lightmap（只影響 rgb）
    if ( (uTextureFlags & 2) != 0 ) {
        vec3 lightMapColor = vec3(1.0f, 1.0f, 1.0f);      
        vec3 lightTex = texture(uLightMap, vLightMapUV).rgb;
        //  加法混合 (Additive Lighting)
        float lightMapStrength = 0.8; // 可用 uniform 調節強度
        finalColor.rgb += lightMapColor * lightMapStrength * lightTex * colorRGB; 
    }

    // cube mapping 立方體環境貼圖 (環境反射)
    if ((uTextureFlags & 8) != 0) {
        float envIntensity = 0.6f; // 環境貼圖對最終顏色的影響程度（0.0～1.0）
        // 反射向量：reflect(-V, N)
        vec3 R = reflect(-V, N);
        R.y = -R.y; // 反射向量的 y 軸翻轉，因為 OpenGL 的環境貼圖是從上往下看
        vec3 envColor = texture(uEnvMap, R).rgb;
        finalColor.rgb = mix(finalColor.rgb, envColor, envIntensity);
    }

    /*float edge = abs(dot(N, V));
    if (edge < 0.2) {
        finalColor = vec4(0.13, 0.1, 0.1, 1.0);
    }*/

    FragColor = vec4(finalColor.rgb, alpha);
}
