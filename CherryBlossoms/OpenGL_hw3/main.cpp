//#define GLM_ENABLE_EXPERIMENTAL 1
/*
設計一個房間，至少包含六個區域，可以不用柱子，直接用兩個四方形做區隔即可，右邊為示意圖，你可以自己設計，綠色表示鏡頭一開始的位置
(12%)畫出六個房間，並達成以下的條件（編譯要能過且正常執行才算）
    v每一面牆都必須有貼圖
    v必須至少有三個房間是使用燈光照明，而且是使用 Per Pixel Lighting
    v每一個房間中都必須有放置裝飾品(部分必須有貼圖)
    v(2%) 必須有房間顯示至少(含)一個 OBJ 檔的模型，而且包含貼圖
(8%)環境與操控
    v(2%) 鏡頭的移動是根據目前的視角方向
    v(2%) 不會穿牆
    v(2%) 會被機關觸動的動態移動光源，使用 Per Pixel Lighting
    v(2%) 至少三面牆壁上半透明的玻璃
(8 %)圖學相關功能的使用，必須在程式碼中以註解清楚標明
    v(1%) 針對特定物件實現 Billboards 的功能
    v(1%) 使用到  Mipmapped 的功能 （有具體的說明在程式碼中）
    v(2%) 有房間使用到 Light Map 的功能 （有具體的說明在程式碼中）
    v(2%) 有物件使用到 Normal Map 的功能 （有具體的說明在程式碼中）
    v(2%) 有物件使用到 Environment Map 的功能 (有具體的說明在程式碼中）
(6%) 其他你覺得可以展示的技術，包含物理或是數學的運算
    v(3%)發射子彈並且在牆壁上留下彈孔
    v(3%)可以破壞房間內的擺設
(4%) 創意分數
老師給分，請自行發揮
門可以開關，並且關起時會阻擋玩家前進
破壞物品有粒子效果
水的效果
鑰匙開門
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <array>

#include <glew/include/GL/glew.h>
#include <glfw/include/GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//#include <glm/gtx/string_cast.hpp>

#include "common/initshader.h"
#include "common/arcball.h"
#include "common/wmhandler.h"
#include "common/CCamera.h"
#include "common/CShaderPool.h"
#include "common/CTexturePool.h"
#include "../models/CQuad.h"
#include "../models/CObj.h"
#include "common/CButton.h"
#include "models/CCube.h"
#include "common/png_loader.h"

#include "common/CLight.h"
#include "common/CMaterial.h"
#include "common/loading.h"
#include "common/bullet.h"
#include "models/CRoom.h"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 800 
#define ROW_NUM 10
#define NUM_LIGHTS 6

//貼圖
TextureData g_texData[48];
TextureData g_waterTex[30];
GLuint g_uiCubeMap, g_uiCubeMap2; // for Cube Texture Object

CRoom room[6]{
    CRoom(9.0f, -9.0f, 9.0f, -9.0f),
    CRoom(9.0f, -9.0f, -9.01f, -27.01f),
    CRoom(9.0f, -9.0f, -27.02f, -45.02f),
    CRoom(-9.01f, -27.01f, 9.0f, -9.0f),
    CRoom(-9.01f, -27.01f, -9.01f, -27.01f),
    CRoom(-9.01f, -27.01f, -27.02f, -45.02f)
};

//obj檔案
CObj g_window[6],
g_doll[6][2],
g_chair,
g_table,
g_door[6][2],
g_bookcase,
g_vase,
g_plant,
g_lightModelone_1,
g_lightModelone_2,
g_lightModeltwo_1,
g_lightModeltwo_2,
g_lightModeltable_1,
g_lightModeltable_2,
g_lightModelthree,
g_lightModelfour_1,
g_lightModelfour_2,
g_target[3],
g_sofa,
g_little_table,
g_tv,
g_black_board,
g_swing[6][2],
g_window2[3],
g_window3[1],
g_window4,
g_tree[4][2],
stone,
g_key,
g_lock[4],
g_ceilingfan, g_ceilingfan_2,
g_gun,
g_sakula;

CQuad w_glass[6][4], w2_glass[2], g_bird[5], water, floor_outside, tvshow, hole[10], hint[5];

//動態控制項
int ani = 0; //water animation
float ani_clock = 0.0f;

float fan_clock = 0.0f;

float flower_clock = 0.0f;
int flower_count = 0;
float flower_size = 0.4f;
float flower_size2 = 0.4f;
bool flower_active = false;

bool shooting = false;
float shooting_clock = 0.0f;

bool lockdisable = false;
float lockdisable_clock = 0.0f;
bool lockdisable2 = false;
float lockdisable2_clock = 0.0f;

CCube break_cube[50], classroom_light, cflower[300];

glm::vec3 g_eyeloc(6.0f, 4.5f, 6.0f); // 鏡頭位置

GLuint g_shadingProg;
GLuint g_nolightShader;
GLuint g_uiShader;

GLint g_3dviewLoc, g_3dProjLoc;
GLint g_2dviewLoc, g_2dProjLoc;

// 全域光源
CLight g_light[10]{
    CLight(glm::vec3(0.0f, 8.5f, 0.0f)),
    CLight(glm::vec3(-9.0f, 5.0f, -19.0f)),
    CLight(glm::vec3(2.0f, 3.7f, -17.0f)),
    CLight(glm::vec3(-10.3f, 5.0f, 4.0f)),
    CLight(glm::vec3(-10.3f, 5.0f, -4.0f)),
    CLight(glm::vec3(-23.0f, 6.0f, -40.0f)),
    CLight(glm::vec3(-21.5f, -1.0f, -17.0f)),

    //互動光源
    CLight(glm::vec3(5.0f, 5.0f, -2.0f)),
    CLight(glm::vec3(-18.0f, 5.0f, -18.0f)),
    CLight(glm::vec3(-4.0f, 5.0f, -24.0f)),
};

// 全域材質（可依模型分別設定）
CMaterial g_matBeige;   // 淺米白深麥灰
CMaterial g_matWaterGreen;
CMaterial g_matWoodHoney;
CMaterial g_matWoodBleached;
CMaterial g_matGlassYellow;
CMaterial g_matPlantGreen;
CMaterial g_matPaper;
CMaterial g_matPotBrickRed;
CMaterial g_matMetalGold;
CMaterial g_matFabricWhiteTranslucent;
CMaterial g_matMatteBlackMetal;
CMaterial g_matClothBeige;
CMaterial g_matGlassPink;
CMaterial g_matGlassSkyBlue;
CMaterial g_matGlassLightGreen;
CMaterial g_matWoodWhite;
CMaterial g_matConcreteWhite;
CMaterial g_matWoodDark;
CMaterial g_matWater;
CMaterial g_matYellowSofa;

//2d介面
std::array<CButton, 2> g_button = {
    CButton(5.0f, 5.0f),
    CButton(40.0f, 50.0f),
};

glm::mat4 g_2dmxView = glm::mat4(1.0f);
glm::mat4 g_2dmxProj = glm::mat4(1.0f);

void genMaterial();

//----------------------------------------------------------------------------
void loadScene(void)
{
    genMaterial();
    g_shadingProg = CShaderPool::getInstance().getShader("v_npr.glsl", "f_npr.glsl");
    g_nolightShader = CShaderPool::getInstance().getShader("v_shader3.glsl", "f_shader3.glsl");
    g_uiShader = CShaderPool::getInstance().getShader("ui_vtxshader.glsl", "ui_fragshader.glsl");

    g_light[0].setShaderID(g_shadingProg, "uLight[0]");
    g_light[0].setAmbient(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
    g_light[0].setDiffuse(glm::vec4(1.0f, 0.85f, 0.5f, 1.0f));
    g_light[0].setAttenuation(1.0f, 0.14f, 0.07f);
    g_light[0].setIntensity(9.0f);
    g_light[0].setTarget(glm::vec3(0.0f, 0.0f, 0.0f));
    g_light[0].setCutOffDeg(20.0f, 130.0f);

    g_light[1].setShaderID(g_shadingProg, "uLight[1]");
    g_light[1].setAmbient(glm::vec4(0.1f, 0.15f, 0.25f, 1.0f)); 
    g_light[1].setDiffuse(glm::vec4(0.4f, 0.6f, 1.0f, 1.0f));
    g_light[1].setAttenuation(1.0f, 0.09f, 0.032f);
    g_light[1].setIntensity(2.5f);
    g_light[1].setTarget(glm::vec3(9.0f, 0.0f, -18.0f));
    g_light[1].setCutOffDeg(45.0f, 180.0f);

    g_light[2].setShaderID(g_shadingProg, "uLight[2]");
    g_light[2].setAmbient(glm::vec4(0.4f, 0.3f, 0.15f, 1.0f));
    g_light[2].setDiffuse(glm::vec4(1.0f, 0.7f, 0.3f, 1.0f));
    g_light[2].setAttenuation(1.0f, 0.3f, 0.032f);
    g_light[2].setIntensity(4.0f);
    g_light[2].setTarget(glm::vec3(-4.0f, 0.0f, -8.1f));
    g_light[2].setCutOffDeg(45.0f, 180.0f);

    g_light[3].setShaderID(g_shadingProg, "uLight[3]");
    g_light[3].setAmbient(glm::vec4(0.4f, 0.3f, 0.15f, 1.0f));
    g_light[3].setDiffuse(glm::vec4(1.0f, 0.7f, 0.3f, 1.0f));
    g_light[3].setAttenuation(1.0f, 0.3f, 0.032f);
    g_light[3].setIntensity(2.5f);
    g_light[3].setTarget(glm::vec3(-10.0f, 0.0f, 0.0f));

    g_light[4].setShaderID(g_shadingProg, "uLight[4]");
    g_light[4].setAmbient(glm::vec4(0.4f, 0.3f, 0.15f, 1.0f));
    g_light[4].setDiffuse(glm::vec4(1.0f, 0.7f, 0.3f, 1.0f));
    g_light[4].setAttenuation(1.0f, 0.3f, 0.032f);
    g_light[4].setIntensity(2.5f);
    g_light[4].setTarget(glm::vec3(-10.0f, 0.0f, 0.0f));

    g_light[5].setShaderID(g_shadingProg, "uLight[5]");
    g_light[5].setAmbient(glm::vec4(0.4f, 0.3f, 0.3f, 1.0f));
    g_light[5].setDiffuse(glm::vec4(1.0f, 0.9f, 0.5f, 1.0f));
    g_light[5].setAttenuation(1.0f, 0.1f, 0.01f);
    g_light[5].setIntensity(4.0f);

    g_light[6].setShaderID(g_shadingProg, "uLight[6]");
    g_light[6].setAmbient(glm::vec4(0.1f, 0.1f, 0.3f, 1.0f));
    g_light[6].setDiffuse(glm::vec4(0.4f, 0.6f, 1.0f, 1.0f));
    g_light[6].setAttenuation(1.0f, 0.5f, 0.2f);
    g_light[6].setIntensity(5.0f);
    g_light[6].setActiveType(1);
    g_light[6].setTargetCenter(glm::vec3(-18.0f, 0.0f, -18.0f));

    g_light[6].setLightObjScale(glm::vec3(0.15f, 0.03f, 0.15f));
    g_light[6]._wing[0].setupVertexAttributes();
    g_light[6]._wing[0].setShaderID(g_shadingProg, 3);
    g_light[6]._wing[0].setMaterial(g_matPaper);
    g_light[6]._wing[0].setScale(glm::vec3(0.1f, 0.01f, 0.1f));
    g_light[6]._wing[0].setPos(glm::vec3(-21.5f, -1.0f, -17.2f));


    //互動光
    g_light[7].setShaderID(g_shadingProg, "uLight[7]");
    g_light[7].setAmbient(glm::vec4(0.1f, 0.1f, 0.3f, 1.0f));
    g_light[7].setDiffuse(glm::vec4(0.4f, 0.6f, 1.0f, 1.0f));
    g_light[7].setAttenuation(1.0f, 0.5f, 0.2f);
    g_light[7].setIntensity(5.0f);
    g_light[7].setActiveType(2);
    g_light[7].setTargetArrive(glm::vec3(3.0f, 5.0f, -37.0f));
    g_light[7].setTargetCenter(glm::vec3(0.0f, 5.0f, -36.0f));

    g_light[7].setLightObjScale(glm::vec3(0.05f, 0.05f, 0.05f));
    g_light[7]._wing[0].setupVertexAttributes();
    g_light[7]._wing[0].setShaderID(g_shadingProg, 3);
    g_light[7]._wing[0].setMaterial(g_matGlassSkyBlue);
    g_light[7]._wing[0].setScale(glm::vec3(0.5f, 0.01f, 0.3f));
    g_light[7]._wing[1].setupVertexAttributes();
    g_light[7]._wing[1].setShaderID(g_shadingProg, 3);
    g_light[7]._wing[1].setMaterial(g_matGlassSkyBlue);
    g_light[7]._wing[1].setScale(glm::vec3(0.5f, 0.01f, 0.3f));


    g_light[8].setShaderID(g_shadingProg, "uLight[8]");
    g_light[8].setAmbient(glm::vec4(0.2f, 0.05f, 0.02f, 1.0f)); // 淡淡的粉橘環境光
    g_light[8].setDiffuse(glm::vec4(1.0f, 0.4f, 0.3f, 1.0f));  // 高飽和粉橘色
    g_light[8].setAttenuation(1.0f, 0.5f, 0.2f);
    g_light[8].setIntensity(5.0f);
    g_light[8].setActiveType(2);
    g_light[8].setTargetArrive(glm::vec3(-3.0f, 5.0f, -35.0f));
    g_light[8].setTargetCenter(glm::vec3(0.0f, 5.0f, -36.0f));

    g_light[8].setLightObjScale(glm::vec3(0.05f, 0.05f, 0.05f));
    g_light[8]._wing[0].setupVertexAttributes();
    g_light[8]._wing[0].setShaderID(g_shadingProg, 3);
    g_light[8]._wing[0].setMaterial(g_matGlassPink);
    g_light[8]._wing[0].setScale(glm::vec3(0.5f, 0.01f, 0.3f));
    g_light[8]._wing[1].setupVertexAttributes();
    g_light[8]._wing[1].setShaderID(g_shadingProg, 3);
    g_light[8]._wing[1].setMaterial(g_matGlassPink);
    g_light[8]._wing[1].setScale(glm::vec3(0.5f, 0.01f, 0.3f));

    g_light[9].setShaderID(g_shadingProg, "uLight[9]");
    g_light[9].setAmbient(glm::vec4(0.1f, 0.2f, 0.05f, 1.0f)); // 偏綠的環境光
    g_light[9].setDiffuse(glm::vec4(0.7f, 1.0f, 0.0f, 1.0f));  // 飽和黃綠（亮萊姆綠）
    g_light[9].setAttenuation(1.0f, 0.5f, 0.2f);
    g_light[9].setIntensity(5.0f);
    g_light[9].setActiveType(2);
    g_light[9].setTargetArrive(glm::vec3(0.0f, 5.0f, -40.0f));
    g_light[9].setTargetCenter(glm::vec3(0.0f, 5.0f, -36.0f));

    g_light[9].setLightObjScale(glm::vec3(0.05f, 0.05f, 0.05f));
    g_light[9]._wing[0].setupVertexAttributes();
    g_light[9]._wing[0].setShaderID(g_shadingProg, 3);
    g_light[9]._wing[0].setMaterial(g_matGlassYellow);
    g_light[9]._wing[0].setScale(glm::vec3(0.5f, 0.01f, 0.3f));
    g_light[9]._wing[1].setupVertexAttributes();
    g_light[9]._wing[1].setShaderID(g_shadingProg, 3);
    g_light[9]._wing[1].setMaterial(g_matGlassYellow);
    g_light[9]._wing[1].setScale(glm::vec3(0.5f, 0.01f, 0.3f));


    // 產生貼圖
    g_texData[0] = CTexturePool::getInstance().getTexture("texture/fabric.png",1); //開啟Mipmapped
    g_texData[1] = CTexturePool::getInstance().getTexture("texture/fabric_normal.png",1);

    g_texData[2] = CTexturePool::getInstance().getTexture("texture/wall.jpg");
    g_texData[46] = CTexturePool::getInstance().getTexture("texture/wall_normal.png");
    g_texData[3] = CTexturePool::getInstance().getTexture("texture/wall_wood.png");
    g_texData[45] = CTexturePool::getInstance().getTexture("texture/wall_wood_normal.png");
    g_texData[5] = CTexturePool::getInstance().getTexture("texture/wall_b.PNG");
    g_texData[31] = CTexturePool::getInstance().getTexture("texture/wall_b_normal.png");
    g_texData[21] = CTexturePool::getInstance().getTexture("texture/wall_white.jpg");
    g_texData[22] = CTexturePool::getInstance().getTexture("texture/wall_white_normal.png");
    g_texData[27] = CTexturePool::getInstance().getTexture("texture/Stone_Pavement_DIFF.png");
    g_texData[28] = CTexturePool::getInstance().getTexture("texture/Stone_Pavement_NRM.png");
    g_texData[34] = CTexturePool::getInstance().getTexture("texture/Marble_Tiles_Diffuse.jpg");
    g_texData[35] = CTexturePool::getInstance().getTexture("texture/Marble_Tiles_Normal.png");
    g_texData[39] = CTexturePool::getInstance().getTexture("texture/wall_blue.jpg");
    g_texData[40] = CTexturePool::getInstance().getTexture("texture/wall_blue_normal.png");
    g_texData[6] = CTexturePool::getInstance().getTexture("texture/Tiles012_512.png", 1); //開啟Mipmapped

    g_texData[8] = CTexturePool::getInstance().getTexture("texture/floor.jpg");
    g_texData[11] = CTexturePool::getInstance().getTexture("texture/floor_normal.png");
    g_texData[25] = CTexturePool::getInstance().getTexture("texture/grass.jpg");
    g_texData[26] = CTexturePool::getInstance().getTexture("texture/grass_normal.png");

    g_texData[9] = CTexturePool::getInstance().getTexture("texture/target.png");
    g_texData[10] = CTexturePool::getInstance().getTexture("texture/target_normal.png");

    g_texData[12] = CTexturePool::getInstance().getTexture("texture/sofa.jpg");
    g_texData[13] = CTexturePool::getInstance().getTexture("texture/sofa_normal.png");

    g_texData[30] = CTexturePool::getInstance().getTexture("texture/blackboard.png");
    g_texData[38] = CTexturePool::getInstance().getTexture("texture/colorLogo.jpg");

    g_texData[7] = CTexturePool::getInstance().getTexture("texture/map.jpg");

    //lightMap
    g_texData[14] = CTexturePool::getInstance().getTexture("texture/leaves.jpg");
    g_texData[15] = CTexturePool::getInstance().getTexture("texture/lightMap2.png");
    g_texData[29] = CTexturePool::getInstance().getTexture("texture/windowLight.jpg");
    g_texData[47] = CTexturePool::getInstance().getTexture("texture/windowLight2.jpg");
    g_texData[32] = CTexturePool::getInstance().getTexture("texture/lightmap_white.PNG");
    g_texData[33] = CTexturePool::getInstance().getTexture("texture/lightmap_classroom.png");

    //cubeMap (Environment map)
    g_texData[16] = CTexturePool::getInstance().getTexture("texture/lightMap2.png");
    g_uiCubeMap = CubeMap_load_SOIL(0);
    g_uiCubeMap2 = CubeMap_load_SOIL(1);

    //平面景物
    g_texData[17] = CTexturePool::getInstance().getTexture("texture/bird1.png"); //bird
    g_texData[18] = CTexturePool::getInstance().getTexture("texture/bird1_normal.png");
    g_texData[19] = CTexturePool::getInstance().getTexture("texture/bird2.png");
    g_texData[20] = CTexturePool::getInstance().getTexture("texture/bird2_normal.png");
    g_texData[23] = CTexturePool::getInstance().getTexture("texture/bird3.png");
    g_texData[24] = CTexturePool::getInstance().getTexture("texture/bird3_normal.png");
    g_texData[4] = CTexturePool::getInstance().getTexture("texture/hole.png");
    g_texData[36] = CTexturePool::getInstance().getTexture("texture/hole_normal.png");
    g_texData[37] = CTexturePool::getInstance().getTexture("texture/hint1.png"); //hint
    g_texData[41] = CTexturePool::getInstance().getTexture("texture/hint2.png");
    g_texData[42] = CTexturePool::getInstance().getTexture("texture/hint3.png");
    g_texData[43] = CTexturePool::getInstance().getTexture("texture/hint4.png");
    g_texData[44] = CTexturePool::getInstance().getTexture("texture/hint5.png");


    //水動畫
    for (int i = 0; i < 30; i++) {
        std::string filename = "texture/water/" + std::to_string(i + 1) + ".png";
        g_waterTex[i] = CTexturePool::getInstance().getTexture(filename);
    }

    bullet_load();
    for (int i = 0; i < 50; i++) {
        break_cube[i].setupVertexAttributes();
        break_cube[i].setShaderID(g_nolightShader);
        break_cube[i].setScale(glm::vec3(0.07f, 0.008f, 0.07f));
        break_cube[i].setActType(1);
    }

    for (int i = 0; i < 300; i++) {
        // 產生隨機單位球內的偏移
        float maxXZRadius = 7.0f;
        float maxYRange = 3.5f;

        glm::vec3 offset;
        do {
            // 在 [-1, 1] 區間隨機取值
            float x = (rand() / (float)RAND_MAX) * 2.0f - 1.0f;
            float y = (rand() / (float)RAND_MAX) * 2.0f - 1.0f;
            float z = (rand() / (float)RAND_MAX) * 2.0f - 1.0f;

            // 重新組合成向量
            offset = glm::vec3(x, y, z);

            // 檢查 XZ 平面內距離是否在允許範圍內
        } while (glm::length(glm::vec2(offset.x, offset.z)) > 1.0f);

        // 縱向 Y 與橫向 XZ 個別縮放
        offset.x *= maxXZRadius;
        offset.z *= maxXZRadius;
        offset.y *= maxYRange;

        // 基準點
        glm::vec3 base = glm::vec3(0.0f, 10.0f, -36.0f);
        glm::vec3 position = base + offset;
        
        cflower[i].setupVertexAttributes();
        cflower[i].setShaderID(g_nolightShader);
        cflower[i].setColor(glm::vec4(1.0f, 0.72f, 0.77f, 1.0f));
        cflower[i].setScale(glm::vec3(0.4f, 0.06f, 0.4f));
        cflower[i].setPos(position);
    }

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 4; j++) {
            room[i].wall[j].setupVertexAttributes();
            room[i].wall[j].setShaderID(g_shadingProg, 3);
            room[i].wall[j].setMaterial(g_matConcreteWhite);
            if (i == 2) {
                if(j == 0 || j == 3) room[i].wall[j].setTextureMode(CShape::TEX_CUBICMAP | CShape::TEX_LIGHTMAP);
                else room[i].wall[j].setTextureMode(CShape::TEX_DIFFUSE | CShape::TEX_NORMALMAP);
            }
            else room[i].wall[j].setTextureMode(CShape::TEX_DIFFUSE | CShape::TEX_NORMALMAP | CShape::TEX_LIGHTMAP);
        }
        room[i].top.setupVertexAttributes();
        room[i].top.setShaderID(g_shadingProg, 3);
        room[i].top.setMaterial(g_matConcreteWhite);

        room[i].ground.setupVertexAttributes();
        room[i].ground.setShaderID(g_shadingProg, 3);
        room[i].ground.setMaterial(g_matClothBeige);
        if (i == 4) {
            room[i].ground.setTextureMode(CShape::TEX_NORMALMAP | CShape::TEX_LIGHTMAP);
            room[i].ground.setMaterial(g_matWater);
        }
        else if(i == 5) room[i].ground.setTextureMode(CShape::TEX_DIFFUSE | CShape::TEX_NORMALMAP | CShape::TEX_LIGHTMAP);
        else if(i == 2) room[i].ground.setTextureMode(CShape::TEX_CUBICMAP | CShape::TEX_LIGHTMAP);
        else room[i].ground.setTextureMode(CShape::TEX_DIFFUSE | CShape::TEX_NORMALMAP);

        for (int j = 0; j < 8; j++) {
            room[i].pillair[j].setupVertexAttributes();
            room[i].pillair[j].setShaderID(g_shadingProg, 3);
            room[i].pillair[j].setMaterial(g_matWoodDark);
        }
    }
    room[0].wall[0].setWallType(2);
    room[0].wall[1].setWallType(1);
    room[0].wall[2].setWallType(2);
    room[0].wall[3].setWallType(1);

    room[1].wall[0].setWallType(2);
    room[1].wall[1].setWallType(0);
    room[1].wall[2].setWallType(1);
    room[1].wall[3].setWallType(1);

    room[2].wall[0].setWallType(0);
    room[2].wall[1].setWallType(1);
    room[2].wall[2].setWallType(1);
    room[2].wall[2].setRotate(180, glm::vec3(0, 1, 0));
    room[2].wall[3].setWallType(0);

    room[3].wall[0].setWallType(1);
    room[3].wall[1].setWallType(0);
    room[3].wall[2].setWallType(2);
    room[3].wall[3].setWallType(1);
    room[3].ground.setLightMapTiling(4, 4);

    room[4].wall[0].setWallType(0);
    room[4].wall[1].setWallType(4);
    room[4].wall[2].setWallType(1);
    room[4].wall[3].setWallType(1);
    for (int i = 0; i < 4; i++) {
        room[4].wall[i].setTiling(4, 4);
    }

    room[5].wall[0].setWallType(1);
    room[5].wall[0].setRotate(90.0f, glm::vec3(0, 1, 0));
    room[5].wall[1].setWallType(3);
    room[5].wall[2].setWallType(1);
    room[5].wall[3].setWallType(3);
    room[5].ground.setTiling(5, 5);
    room[5].ground.setLightMapTiling(2, 2);
    
    floor_outside.setupVertexAttributes();
    floor_outside.setShaderID(g_shadingProg, 3);
    floor_outside.setScale(glm::vec3(4.0f, 40.0f, 1.0f));
    floor_outside.setRotate(-90, glm::vec3(1, 0, 0));
    floor_outside.setTextureMode(CShape::TEX_DIFFUSE | CShape::TEX_LIGHTMAP);
    floor_outside.setTiling(1, 10);
    floor_outside.setMaterial(g_matClothBeige);
    floor_outside.setPos(glm::vec3(-18.0f, 3.8f, 30.0f));

    tvshow.setupVertexAttributes();
    tvshow.setShaderID(g_shadingProg, 3);
    tvshow.setScale(glm::vec3(6.0f, 3.5f, 1.0f));
    tvshow.setRotate(90, glm::vec3(0, 1, 0));
    tvshow.setTextureMode(CShape::TEX_DIFFUSE | CShape::TEX_LIGHTMAP);
    tvshow.setMaterial(g_matClothBeige);
    tvshow.setPos(glm::vec3(-8.15f, 5.8f, -18.8f));


    // Read our .obj file
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    std::vector<GLfloat> extras;

    //doll--------------------------------------------------
    bool res = loadOBJ("models/doll.obj", vertices, uvs, normals, indices, extras);
    g_doll[0][0].Load(vertices, uvs, normals, indices, extras);
    g_doll[0][0].setupVertexAttributes();
    g_doll[0][0].setShaderID(g_shadingProg, 3);
    g_doll[0][0].setScale(glm::vec3(2.0f, 2.0f, 2.0f));
    g_doll[0][0].setTextureMode(CShape::TEX_DIFFUSE | CShape::TEX_NORMALMAP);
    g_doll[0][0].setMaterial(g_matClothBeige);
    g_doll[0][0].setRadius(2.0f);
    g_doll[0][0].setPos(glm::vec3(0.0f, 0.0f, -1.0f));

    res = loadOBJ("models/doll_2.obj", vertices, uvs, normals, indices, extras);
    g_doll[0][1].Load(vertices, uvs, normals, indices, extras);
    g_doll[0][1].setupVertexAttributes();
    g_doll[0][1].setShaderID(g_shadingProg, 3);
    g_doll[0][1].setScale(glm::vec3(2.0f, 2.0f, 2.0f));
    g_doll[0][1].setPos(glm::vec3(0.0f, 0.0f, -1.0f));
    g_doll[0][1].setMaterial(g_matMatteBlackMetal); 
    
    res = loadOBJ("models/doll_chairseat.obj", vertices, uvs, normals, indices, extras);
    g_doll[1][0].Load(vertices, uvs, normals, indices, extras);
    g_doll[1][0].setupVertexAttributes();
    g_doll[1][0].setShaderID(g_shadingProg, 3);
    g_doll[1][0].setScale(glm::vec3(2.0f, 2.0f, 2.0f));
    g_doll[1][0].setTextureMode(CShape::TEX_DIFFUSE | CShape::TEX_NORMALMAP);
    g_doll[1][0].setRotate(-90.0f, glm::vec3(0, 1, 0));
    g_doll[1][0].setPos(glm::vec3(4.7f, 1.8f, -19.0f));
    g_doll[1][0].setMaterial(g_matClothBeige);

    res = loadOBJ("models/doll_chairseat_2.obj", vertices, uvs, normals, indices, extras);
    g_doll[1][1].Load(vertices, uvs, normals, indices, extras);
    g_doll[1][1].setupVertexAttributes();
    g_doll[1][1].setShaderID(g_shadingProg, 3);
    g_doll[1][1].setScale(glm::vec3(2.0f, 2.0f, 2.0f));
    g_doll[1][1].setPos(glm::vec3(4.7f, 1.8f, -19.0f));
    g_doll[1][1].setRotate(-90.0f, glm::vec3(0, 1, 0));
    g_doll[1][1].setMaterial(g_matMatteBlackMetal);

    res = loadOBJ("models/doll_standflower.obj", vertices, uvs, normals, indices, extras);
    g_doll[2][0].Load(vertices, uvs, normals, indices, extras);
    g_doll[2][0].setupVertexAttributes();
    g_doll[2][0].setShaderID(g_shadingProg, 3);
    g_doll[2][0].setScale(glm::vec3(2.0f, 2.0f, 2.0f));
    g_doll[2][0].setTextureMode(CShape::TEX_DIFFUSE | CShape::TEX_NORMALMAP);
    g_doll[2][0].setRotate(-45.0f, glm::vec3(0, 1, 0));
    g_doll[2][0].setPos(glm::vec3(-18.0f, 0.0f, -40.0f));
    g_doll[2][0].setMaterial(g_matClothBeige);

    res = loadOBJ("models/doll_standflower_2.obj", vertices, uvs, normals, indices, extras);
    g_doll[2][1].Load(vertices, uvs, normals, indices, extras);
    g_doll[2][1].setupVertexAttributes();
    g_doll[2][1].setShaderID(g_shadingProg, 3);
    g_doll[2][1].setScale(glm::vec3(2.0f, 2.0f, 2.0f));
    g_doll[2][1].setPos(glm::vec3(-18.0f, 0.0f, -40.0f));
    g_doll[2][1].setRotate(-45.0f, glm::vec3(0, 1, 0));
    g_doll[2][1].setMaterial(g_matMatteBlackMetal);

    res = loadOBJ("models/doll_stand.obj", vertices, uvs, normals, indices, extras);
    g_doll[3][0].Load(vertices, uvs, normals, indices, extras);
    g_doll[3][0].setupVertexAttributes();
    g_doll[3][0].setShaderID(g_shadingProg, 3);
    g_doll[3][0].setScale(glm::vec3(2.0f, 2.0f, 2.0f));
    g_doll[3][0].setTextureMode(CShape::TEX_DIFFUSE | CShape::TEX_NORMALMAP);
    g_doll[3][0].setPos(glm::vec3(-22.0f, 0.0f, 3.0f));
    g_doll[3][0].setRotate(150.0f, glm::vec3(0, 1, 0));
    g_doll[3][0].setMaterial(g_matClothBeige);

    res = loadOBJ("models/doll_stand_2.obj", vertices, uvs, normals, indices, extras);
    g_doll[3][1].Load(vertices, uvs, normals, indices, extras);
    g_doll[3][1].setupVertexAttributes();
    g_doll[3][1].setShaderID(g_shadingProg, 3);
    g_doll[3][1].setScale(glm::vec3(2.0f, 2.0f, 2.0f));
    g_doll[3][1].setPos(glm::vec3(-22.0f, 0.0f, 3.0f));
    g_doll[3][1].setRotate(150.0f, glm::vec3(0, 1, 0));
    g_doll[3][1].setMaterial(g_matMatteBlackMetal);

    res = loadOBJ("models/doll_chair2seat.obj", vertices, uvs, normals, indices, extras);
    g_doll[4][0].Load(vertices, uvs, normals, indices, extras);
    g_doll[4][0].setupVertexAttributes();
    g_doll[4][0].setShaderID(g_shadingProg, 3);
    g_doll[4][0].setScale(glm::vec3(1.5f, 1.5f, 1.5f));
    g_doll[4][0].setTextureMode(CShape::TEX_DIFFUSE | CShape::TEX_NORMALMAP);
    g_doll[4][0].setPos(glm::vec3(2.8f, 1.5f, -35.0f));
    g_doll[4][0].setRotate(-30.0f, glm::vec3(0, 1, 0));
    g_doll[4][0].setMaterial(g_matClothBeige);

    res = loadOBJ("models/doll_chair2seat_2.obj", vertices, uvs, normals, indices, extras);
    g_doll[4][1].Load(vertices, uvs, normals, indices, extras);
    g_doll[4][1].setupVertexAttributes();
    g_doll[4][1].setShaderID(g_shadingProg, 3);
    g_doll[4][1].setScale(glm::vec3(1.5f, 1.5f, 1.5f));
    g_doll[4][1].setPos(glm::vec3(2.8f, 1.5f, -35.0f)); 
    g_doll[4][1].setRotate(-30.0f, glm::vec3(0, 1, 0));
    g_doll[4][1].setMaterial(g_matMatteBlackMetal);

    //door------------------------------------------------
    res = loadOBJ("models/door01.obj", vertices, uvs, normals, indices, extras);
    for (int i = 0; i < 6; i++) {
        g_door[i][0].Load(vertices, uvs, normals, indices, extras);
        g_door[i][0].setupVertexAttributes();
        g_door[i][0].setShaderID(g_shadingProg, 3);
        g_door[i][0].setScale(glm::vec3(0.05f, 0.05f, 0.05f));
        g_door[i][0].setMaterial(g_matWoodHoney);      
    }
    g_door[0][0].setPos(glm::vec3(0.5f, 4.0f, -9.0f));
    g_door[1][0].setPos(glm::vec3(0.5f, 4.0f, -27.01f));
    g_door[2][0].setPos(glm::vec3(-17.5f, 4.0f, -27.01f));
    g_door[3][0].setPos(glm::vec3(-17.5f, 4.0f, -9.01f));

    g_door[4][0].setPos(glm::vec3(-8.6f, 4.0f, 0.0f));
    g_door[4][0].setRotate(90.0f, glm::vec3(0, 1, 0));

    g_door[5][0].setPos(glm::vec3(-8.6f, 4.0f, -36.0f));
    g_door[5][0].setRotate(90.0f, glm::vec3(0, 1, 0));

    res = loadOBJ("models/door02.obj", vertices, uvs, normals, indices, extras);
    for (int i = 0; i < 4; i++) {
        g_door[i][1].Load(vertices, uvs, normals, indices, extras);
        g_door[i][1].setupVertexAttributes();
        g_door[i][1].setShaderID(g_shadingProg, 3);
        g_door[i][1].setScale(glm::vec3(0.05f, 0.05f, 0.05f));
        g_door[i][1].setMaterial(g_matWoodHoney);
        g_door[i][1].setRadius(2.0f);
    }

    res = loadOBJ("models/door02_2.obj", vertices, uvs, normals, indices, extras);
    for (int i = 4; i < 6; i++) {
        g_door[i][1].Load(vertices, uvs, normals, indices, extras);
        g_door[i][1].setupVertexAttributes();
        g_door[i][1].setShaderID(g_shadingProg, 3);
        g_door[i][1].setScale(glm::vec3(0.05f, 0.05f, 0.05f));
        g_door[i][1].setMaterial(g_matWoodHoney);
        g_door[i][1].setRadius(2.0f);
    }

    g_door[0][1].setPos(glm::vec3(0.5f, 4.0f, -9.0f));
    g_door[0][1].setPivot(glm::vec3(-1.2f,0.0f,0.0f));

    g_door[1][1].setPos(glm::vec3(0.5f, 4.0f, -27.01f));
    g_door[1][1].setPivot(glm::vec3(-1.2f, 0.0f, 0.0f));
    g_door[1][1].setDoorActive(false);

    g_door[2][1].setPos(glm::vec3(-17.5f, 4.0f, -27.01f));
    g_door[2][1].setPivot(glm::vec3(-1.2f, 0.0f, 0.0f));

    g_door[3][1].setPos(glm::vec3(-17.5f, 4.0f, -9.01f));
    g_door[3][1].setPivot(glm::vec3(-1.2f, 0.0f, 0.0f));

    g_door[4][1].setPivot(glm::vec3(0.0f, 0.0f, 1.2f));
    g_door[4][1].setPos(glm::vec3(-8.9f, 4.0f, -0.5f));

    g_door[5][1].setPivot(glm::vec3(0.0f, 0.0f, 1.2f));
    g_door[5][1].setPos(glm::vec3(-8.9f, 4.0f, -36.4f));
    g_door[5][1].setDoorActive(false);

    //other model--------------------------------------------------------
    res = loadOBJ("models/swing.obj", vertices, uvs, normals, indices, extras);
    for (int i = 0; i < 6; i++) {
        g_swing[i][0].Load(vertices, uvs, normals, indices, extras);
        g_swing[i][0].setupVertexAttributes();
        g_swing[i][0].setShaderID(g_shadingProg, 3);
        g_swing[i][0].setMaterial(g_matWoodDark);
        g_swing[i][0].setRadius(2.0f);
    }

    res = loadOBJ("models/swing_2.obj", vertices, uvs, normals, indices, extras);
    for (int i = 0; i < 6; i++) {
        g_swing[i][1].Load(vertices, uvs, normals, indices, extras);
        g_swing[i][1].setupVertexAttributes();
        g_swing[i][1].setShaderID(g_shadingProg, 3);
        g_swing[i][1].setMaterial(g_matPaper);
        g_swing[i][1].setRadius(2.0f);
    }

    g_swing[0][0].setRotate(45.0f, glm::vec3(0, 1, 0));
    g_swing[0][1].setRotate(45.0f, glm::vec3(0, 1, 0));
    g_swing[0][0].setPos(glm::vec3(-12.0f, 7.0f, -40.0f));
    g_swing[0][1].setPos(glm::vec3(-12.0f, 7.0f, -40.0f));

    g_swing[1][0].setRotate(30.0f, glm::vec3(0, 1, 0));
    g_swing[1][1].setRotate(30.0f, glm::vec3(0, 1, 0));
    g_swing[1][0].setPos(glm::vec3(-17.0f, 5.0f, -43.0f));
    g_swing[1][1].setPos(glm::vec3(-17.0f, 5.0f, -43.0f));

    g_swing[2][0].setRotate(30.0f, glm::vec3(0, 1, 0));
    g_swing[2][1].setRotate(30.0f, glm::vec3(0, 1, 0));
    g_swing[2][0].setPos(glm::vec3(-24.0f, 6.0f, -30.0f));
    g_swing[2][1].setPos(glm::vec3(-24.0f, 6.0f, -30.0f)); 

    g_swing[3][0].setScale(glm::vec3(0.7f, 0.7f, 0.7f));
    g_swing[3][1].setScale(glm::vec3(0.7f, 0.38f, 0.7f));
    g_swing[3][0].setRotate(60.0f, glm::vec3(0, 1, 0));
    g_swing[3][1].setRotate(60.0f, glm::vec3(0, 1, 0));
    g_swing[3][0].setPos(glm::vec3(2.8f, 2.0f, -34.6f));
    g_swing[3][1].setPos(glm::vec3(2.8f, 2.0f, -34.6f));

    res = loadOBJ("models/window2.obj", vertices, uvs, normals, indices, extras);
    for (int i = 0; i < 2; i++) {
        g_window2[i].Load(vertices, uvs, normals, indices, extras);
        g_window2[i].setupVertexAttributes();
        g_window2[i].setShaderID(g_shadingProg, 3);
        g_window2[i].setScale(glm::vec3(1.2f, 1.2f, 1.2f));
        if(i == 2) g_window2[i].setMaterial(g_matWoodWhite);
        g_window2[i].setMaterial(g_matWaterGreen);
    }
    g_window2[0].setPos(glm::vec3(-27.0f, 0.0f, -36.0f));
    g_window2[1].setRotate(-90.0f, glm::vec3(0, 1, 0));
    g_window2[1].setPos(glm::vec3(-17.5f, 0.0f, -45.4f));

    res = loadOBJ("models/window3.obj", vertices, uvs, normals, indices, extras);
    for (int i = 0; i < 1; i++) {
        g_window3[i].Load(vertices, uvs, normals, indices, extras);
        g_window3[i].setupVertexAttributes();
        g_window3[i].setShaderID(g_shadingProg, 3);
        g_window3[i].setScale(glm::vec3(1.2f, 1.2f, 1.2f));
        g_window3[i].setMaterial(g_matWoodWhite);
    }
    g_window3[0].setPos(glm::vec3(-27.0f, 3.0f, -18.0f));

    res = loadOBJ("models/window4.obj", vertices, uvs, normals, indices, extras);
    g_window4.Load(vertices, uvs, normals, indices, extras);
    g_window4.setupVertexAttributes();
    g_window4.setShaderID(g_shadingProg, 3);
    g_window4.setScale(glm::vec3(1.7f, 1.7f, 1.7f));
    g_window4.setMaterial(g_matWoodWhite);
    g_window4.setPos(glm::vec3(0.0f, 0.0f, -36.0f));

    res = loadOBJ("models/window.obj", vertices, uvs, normals, indices, extras);
    for (int i = 0; i < 6; i++) {
        g_window[i].Load(vertices, uvs, normals, indices, extras);
        g_window[i].setupVertexAttributes();
        g_window[i].setShaderID(g_shadingProg, 3);
        g_window[i].setScale(glm::vec3(0.6f, 0.6f, 0.6f));
        g_window[i].setMaterial(g_matWoodWhite);
    }
    g_window[0].setPos(glm::vec3(0.5f, 6.5f, 9.5f));
    g_window[0].setRotate(90, glm::vec3(0, 1, 0));

    g_window[1].setPos(glm::vec3(10.0f, 6.5f, 0.05f));
    g_window[1].setRotate(180, glm::vec3(0, 1, 0));

    g_window[2].setPos(glm::vec3(-17.5f, 6.5f, 9.5f));
    g_window[2].setRotate(90, glm::vec3(0, 1, 0));

    g_window[3].setPos(glm::vec3(10.0f, 6.5f, -18.05f));
    g_window[3].setRotate(180, glm::vec3(0, 1, 0));

    res = loadOBJ("models/target.obj", vertices, uvs, normals, indices, extras);
    for (int i = 0; i < 3; i++) {
        g_target[i].Load(vertices, uvs, normals, indices, extras);
        g_target[i].setupVertexAttributes();
        g_target[i].setShaderID(g_shadingProg, 3);
        g_target[i].setScale(glm::vec3(1.0f, 1.0f, 1.0f));
        g_target[i].setTextureMode(CShape::TEX_DIFFUSE | CShape::TEX_NORMALMAP);
        g_target[i].setMaterial(g_matConcreteWhite);
        g_target[i].setRadius(0.7f);
        g_target[i].setTargetAction();
    }

    g_target[0].setRotate(-130, glm::vec3(0, 1, 0));
    g_target[0].setPos(glm::vec3(5.0f, 5.0f, -2.0f)); 

    g_target[1].setRotate(90, glm::vec3(0, 1, 0));
    g_target[1].setPos(glm::vec3(-18.0f, 5.0f, -18.0f));

    g_target[2].setRotate(130, glm::vec3(0, 1, 0));
    g_target[2].setPos(glm::vec3(-4.0f, 5.0f, -24.0f));

    for (int i = 0; i < 6; i++) {
        w_glass[i][0].setupVertexAttributes();
        w_glass[i][0].setShaderID(g_shadingProg, 3);
        w_glass[i][0].setScale(glm::vec3(2.1f, 2.5f, 1.0f));
        w_glass[i][0].setMaterial(g_matGlassYellow);

        w_glass[i][1].setupVertexAttributes();
        w_glass[i][1].setShaderID(g_shadingProg, 3);
        w_glass[i][1].setScale(glm::vec3(2.1f, 2.5f, 1.0f));
        w_glass[i][1].setMaterial(g_matGlassSkyBlue);

        w_glass[i][2].setupVertexAttributes();
        w_glass[i][2].setShaderID(g_shadingProg, 3);
        w_glass[i][2].setScale(glm::vec3(2.1f, 2.5f, 1.0f));
        w_glass[i][2].setMaterial(g_matGlassPink);

        w_glass[i][3].setupVertexAttributes();
        w_glass[i][3].setShaderID(g_shadingProg, 3);
        w_glass[i][3].setScale(glm::vec3(2.1f, 2.5f, 1.0f));
        w_glass[i][3].setMaterial(g_matGlassLightGreen);
    }           

    w_glass[0][0].setPos(glm::vec3(-1.1f, 7.7f, 9.0f));
    w_glass[0][1].setPos(glm::vec3(1.1f, 7.7f, 9.0f));
    w_glass[0][2].setPos(glm::vec3(-1.1f, 5.3f, 9.0f));
    w_glass[0][3].setPos(glm::vec3(1.1f, 5.3f, 9.0f));

    w_glass[1][0].setRotate(90.0f, glm::vec3(0, 1, 0));
    w_glass[1][1].setRotate(90.0f, glm::vec3(0, 1, 0));
    w_glass[1][2].setRotate(90.0f, glm::vec3(0, 1, 0));
    w_glass[1][3].setRotate(90.0f, glm::vec3(0, 1, 0));
    w_glass[1][0].setPos(glm::vec3(9.0f, 7.7f, -1.1f));
    w_glass[1][1].setPos(glm::vec3(9.0f, 7.7f, 1.1f));
    w_glass[1][2].setPos(glm::vec3(9.0f, 5.3f, -1.1f));
    w_glass[1][3].setPos(glm::vec3(9.0f, 5.3f, 1.1f));

    for (int i = 0; i < 2; i++) {
        w2_glass[i].setupVertexAttributes();
        w2_glass[i].setShaderID(g_shadingProg, 3);
        w2_glass[i].setScale(glm::vec3(10.0f, 8.5f, 1.0f));
        w2_glass[i].setTextureMode(CShape::TEX_CUBICMAP);
        w2_glass[i].setMaterial(g_matGlassLightGreen);
    }
    w2_glass[0].setPos(glm::vec3(-18.0f, 4.5f, -45.1f));
    w2_glass[1].setRotate(90.0f, glm::vec3(0,1,0));
    w2_glass[1].setPos(glm::vec3(-27.0f, 4.5f, -36.0f));

    res = loadOBJ("models/sofa.obj", vertices, uvs, normals, indices, extras);
    g_sofa.Load(vertices, uvs, normals, indices, extras);
    g_sofa.setupVertexAttributes();
    g_sofa.setShaderID(g_shadingProg, 3);
    g_sofa.setRotate(-90.0f, glm::vec3(0, 1, 0));
    g_sofa.setScale(glm::vec3(0.8f, 0.8f, 0.8f));
    g_sofa.setPos(glm::vec3(6.0f, 0.0f, -18.0f));
    g_sofa.setMaterial(g_matYellowSofa);

    res = loadOBJ("models/little_table.obj", vertices, uvs, normals, indices, extras);
    g_little_table.Load(vertices, uvs, normals, indices, extras);
    g_little_table.setupVertexAttributes();
    g_little_table.setShaderID(g_shadingProg, 3);
    g_little_table.setScale(glm::vec3(5.0f, 4.0f, 5.0f));
    g_little_table.setPos(glm::vec3(2.0f, 0.0f, -17.0f));
    g_little_table.setMaterial(g_matWoodBleached);

    res = loadOBJ("models/blackboard.obj", vertices, uvs, normals, indices, extras);
    g_black_board.Load(vertices, uvs, normals, indices, extras);
    g_black_board.setupVertexAttributes();
    g_black_board.setShaderID(g_shadingProg, 3);
    g_black_board.setScale(glm::vec3(45.0f, 45.0f, 45.0f));
    g_black_board.setRotate(-90.0f, glm::vec3(0, 1, 0));
    g_black_board.setPos(glm::vec3(-24.5f, 0.0f, 0.0f));
    g_black_board.setTextureMode(CShape::TEX_DIFFUSE);
    g_black_board.setMaterial(g_matWoodWhite);
    g_black_board.setRadius(4.0f);

    res = loadOBJ("models/tv.obj", vertices, uvs, normals, indices, extras);
    g_tv.Load(vertices, uvs, normals, indices, extras);
    g_tv.setupVertexAttributes();
    g_tv.setShaderID(g_shadingProg, 3);
    g_tv.setScale(glm::vec3(0.7f, 0.7f, 0.7f));
    g_tv.setRotate(90.0f, glm::vec3(0, 1, 0));
    g_tv.setPos(glm::vec3(-8.0f, 3.0f, -18.0f));
    g_tv.setMaterial(g_matMatteBlackMetal);
    g_tv.setRadius(2.0f);
    
    res = loadOBJ("models/vase.obj", vertices, uvs, normals, indices, extras);
    g_vase.Load(vertices, uvs, normals, indices, extras);
    g_vase.setupVertexAttributes();
    g_vase.setShaderID(g_shadingProg, 3);
    g_vase.setPos(glm::vec3(-21.5f, 0.0f, -38.0f));
    g_vase.setScale(glm::vec3(4.5f, 4.5f, 4.5f));
    g_vase.setMaterial(g_matPotBrickRed);

    res = loadOBJ("models/tree.obj", vertices, uvs, normals, indices, extras);
    for (int i = 0; i < 4; i++) {
        g_tree[i][0].Load(vertices, uvs, normals, indices, extras);
        g_tree[i][0].setupVertexAttributes();
        g_tree[i][0].setShaderID(g_shadingProg, 3);
        g_tree[i][0].setMaterial(g_matPlantGreen);
    }

    res = loadOBJ("models/tree_2.obj", vertices, uvs, normals, indices, extras);
    for (int i = 0; i < 4; i++) {
        g_tree[i][1].Load(vertices, uvs, normals, indices, extras);
        g_tree[i][1].setupVertexAttributes();
        g_tree[i][1].setShaderID(g_shadingProg, 3);
        g_tree[i][1].setMaterial(g_matWoodHoney);
    }
    g_tree[0][0].setScale(glm::vec3(0.8f, 0.8f, 0.8f));
    g_tree[0][1].setScale(glm::vec3(0.8f, 0.8f, 0.8f));
    g_tree[0][0].setPos(glm::vec3(-21.5f, 1.2f, -38.0f));
    g_tree[0][1].setPos(glm::vec3(-21.5f, 1.2f, -38.0f));

    g_tree[1][0].setScale(glm::vec3(4.5f, 4.5f, 4.5f));
    g_tree[1][1].setScale(glm::vec3(4.5f, 4.5f, 4.5f));
    g_tree[1][0].setRotate(90.0f, glm::vec3(0, 1, 0));
    g_tree[1][1].setRotate(90.0f, glm::vec3(0, 1, 0));
    g_tree[1][0].setPos(glm::vec3(-23.0f, 0.0f, -41.0f));
    g_tree[1][1].setPos(glm::vec3(-23.0f, 0.0f, -41.0f));

    g_tree[2][0].setScale(glm::vec3(1.5f, 1.5f, 1.5f));
    g_tree[2][1].setScale(glm::vec3(1.5f, 1.5f, 1.5f));
    g_tree[2][0].setPos(glm::vec3(-12.0f, 0.0f, -30.0f));
    g_tree[2][1].setPos(glm::vec3(-12.0f, 0.0f, -30.0f));

    g_tree[3][0].setScale(glm::vec3(6.0f,6.0f,6.0f));
    g_tree[3][1].setScale(glm::vec3(6.0f, 6.0f, 6.0f));
    g_tree[3][0].setPos(glm::vec3(-18.0f, 3.8f, 50.0f));
    g_tree[3][1].setPos(glm::vec3(-18.0f, 3.8f, 50.0f));

    res = loadOBJ("models/stone.obj", vertices, uvs, normals, indices, extras);
    stone.Load(vertices, uvs, normals, indices, extras);
    stone.setupVertexAttributes();
    stone.setShaderID(g_shadingProg, 3);
    stone.setPos(glm::vec3(-18.0f, 1.5f, -18.0f));
    stone.setMaterial(g_matClothBeige);

    res = loadOBJ("models/sakula.obj", vertices, uvs, normals, indices, extras);
    g_sakula.Load(vertices, uvs, normals, indices, extras);
    g_sakula.setupVertexAttributes();
    g_sakula.setShaderID(g_shadingProg, 3);
    g_sakula.setPos(glm::vec3(0.0f, 0.0f, -36.0f));
    g_sakula.setScale(glm::vec3(6.0f, 6.0f, 6.0f));
    g_sakula.setMaterial(g_matWoodHoney);

    //平面圖型
    for (int i = 0; i < 5; i++) {
        g_bird[i].setupVertexAttributes();
        g_bird[i].setShaderID(g_shadingProg, 3);
        g_bird[i].setScale(glm::vec3(1.6f, 1.6f, 1.6f));
        g_bird[i].setMaterial(g_matBeige);
        g_bird[i].setTextureMode(CShape::TEX_DIFFUSE | CShape::TEX_NORMALMAP);
    }

    g_bird[0].setRotate(45.0f, glm::vec3(0, 1, 0));
    g_bird[0].setPos(glm::vec3(-12.0f, 7.4f, -39.0f));

    g_bird[1].setRotate(45.0f, glm::vec3(0, 1, 0));
    g_bird[1].setPos(glm::vec3(-12.5f, 7.55f, -40.0f));

    g_bird[2].setRotate(45.0f, glm::vec3(0, 1, 0));
    g_bird[2].setPos(glm::vec3(-13.0f, 7.7f, -41.0f));

    g_bird[3].setRotate(45.0f, glm::vec3(0, 1, 0));
    g_bird[3].setPos(glm::vec3(-24.0f, 6.4f, -31.0f));

    g_bird[4].setRotate(45.0f, glm::vec3(0, 1, 0));
    g_bird[4].setPos(glm::vec3(-23.5f, 6.55f, -29.0f));

    for (int i = 0; i < 5; i++) {
        hint[i].setupVertexAttributes();
        hint[i].setShaderID(g_shadingProg, 3);
        hint[i].setScale(glm::vec3(3.0f, 3.0f, 3.0f));
        hint[i].setMaterial(g_matBeige);
        hint[i].setTextureMode(CShape::TEX_DIFFUSE | CShape::TEX_LIGHTMAP);
    }
    hint[0].setPos(glm::vec3(0.0f, 5.5f, 0.0f));
    hint[1].setPos(glm::vec3(-24.0f, 5.5f, 3.0f));
    hint[2].setPos(glm::vec3(5.0f, 7.5f, -18.0f));
    hint[3].setPos(glm::vec3(-18.0f, 6.5f, -37.0f));
    hint[4].setPos(glm::vec3(2.0f, 6.5f, -35.0f));

    for (int i = 0; i < 10; i++) {
        hole[i].setupVertexAttributes();
        hole[i].setShaderID(g_shadingProg, 3);
        hole[i].setScale(glm::vec3(0.8f, 0.8f, 0.8f));
        hole[i].setMaterial(g_matBeige);
        hole[i].setTextureMode(CShape::TEX_DIFFUSE | CShape::TEX_NORMALMAP);
    }

    water.setupVertexAttributes();
    water.setShaderID(g_shadingProg, 3);
    water.setScale(glm::vec3(5.0f, 5.0f, 5.0f));
    water.setRotate(90.0f, glm::vec3(1, 0, 0));
    water.setMaterial(g_matGlassSkyBlue);
    water.setTextureMode(CShape::TEX_NORMALMAP | CShape::TEX_CUBICMAP);
    water.setPos(glm::vec3(-18.0f, 1.0f, -20.0f));

    res = loadOBJ("models/table.obj", vertices, uvs, normals, indices, extras);
    g_table.Load(vertices, uvs, normals, indices, extras);
    g_table.setupVertexAttributes();
    g_table.setShaderID(g_shadingProg, 3);
    g_table.setScale(glm::vec3(1.1f, 1.1f, 1.1f));
    g_table.setPos(glm::vec3(-18.0f, 0.0f, 0.0f));
    g_table.setMaterial(g_matWoodHoney);

    res = loadOBJ("models/chair.obj", vertices, uvs, normals, indices, extras);
    g_chair.Load(vertices, uvs, normals, indices, extras);
    g_chair.setupVertexAttributes();
    g_chair.setShaderID(g_shadingProg, 3);
    g_chair.setScale(glm::vec3(60.0f, 60.0f, 60.0f));
    g_chair.setRotate(-90, glm::vec3(0, 1, 0));
    g_chair.setPos(glm::vec3(-15.0f, 2.0f, 0.0f));
    g_chair.setMaterial(g_matWoodHoney);

    res = loadOBJ("models/ceilingfan.obj", vertices, uvs, normals, indices, extras);
    g_ceilingfan.Load(vertices, uvs, normals, indices, extras);
    g_ceilingfan.setupVertexAttributes();
    g_ceilingfan.setShaderID(g_shadingProg, 3);
    g_ceilingfan.setScale(glm::vec3(0.015f, 0.015f, 0.015f));
    g_ceilingfan.setPos(glm::vec3(-18.0f, 9.5f, 0.0f));
    g_ceilingfan.setMaterial(g_matMetalGold);

    res = loadOBJ("models/ceilingfan_2.obj", vertices, uvs, normals, indices, extras);
    g_ceilingfan_2.Load(vertices, uvs, normals, indices, extras);
    g_ceilingfan_2.setupVertexAttributes();
    g_ceilingfan_2.setShaderID(g_shadingProg, 3);
    g_ceilingfan_2.setScale(glm::vec3(0.015f, 0.015f, 0.015f));
    g_ceilingfan_2.setPos(glm::vec3(-17.7f, 9.5f, 0.0f));
    g_ceilingfan_2.setMaterial(g_matWoodDark);

    classroom_light.setupVertexAttributes();
    classroom_light.setShaderID(g_shadingProg, 3);
    classroom_light.setScale(glm::vec3(0.2f, 0.05f, 18.0f));
    classroom_light.setTextureMode(CShape::TEX_LIGHTMAP);
    classroom_light.setMaterial(g_matWoodWhite);   
    classroom_light.setPos(glm::vec3(-26.4f, 12.0f, 0.0f));

    res = loadOBJ("models/gun.obj", vertices, uvs, normals, indices, extras);
    g_gun.Load(vertices, uvs, normals, indices, extras);
    g_gun.setupVertexAttributes();
    g_gun.setShaderID(g_shadingProg, 3);
    g_gun.setTextureMode(CShape::TEX_DIFFUSE);
    g_gun.setScale(glm::vec3(0.1f, 0.1f, 0.1f));
    g_gun.setPos(glm::vec3(-16.7f, 2.0f, 0.0f));
    g_gun.setMaterial(g_matWoodDark);

    res = loadOBJ("models/key.obj", vertices, uvs, normals, indices, extras);
    g_key.Load(vertices, uvs, normals, indices, extras);
    g_key.setupVertexAttributes();
    g_key.setShaderID(g_shadingProg, 3);
    g_key.setScale(glm::vec3(40.0f, 40.0f, 40.0f));
    g_key.setPos(glm::vec3(-18.0f, 0.5f, -36.0f));
    g_key.setMaterial(g_matMetalGold);

    res = loadOBJ("models/lock.obj", vertices, uvs, normals, indices, extras);
    for (int i = 0; i < 4; i++) {
        g_lock[i].Load(vertices, uvs, normals, indices, extras);
        g_lock[i].setupVertexAttributes();
        g_lock[i].setShaderID(g_shadingProg, 3);
        g_lock[i].setScale(glm::vec3(0.2f, 0.2f, 0.2f));
        g_lock[i].setMaterial(g_matClothBeige);
    }
    g_lock[0].setPos(glm::vec3(-9.2f, 3.0f, -37.5f));
    g_lock[1].setPos(glm::vec3(-8.2f, 3.0f, -37.5f));

    g_lock[2].setRotate(90.0f, glm::vec3(0, 1, 0));
    g_lock[3].setRotate(90.0f, glm::vec3(0, 1, 0));
    g_lock[2].setPos(glm::vec3(2.0f, 3.0f, -26.2f));
    g_lock[3].setPos(glm::vec3(2.0f, 3.0f, -27.0f));


    //lightmodel------------------------------------------------

    res = loadOBJ("models/light03.obj", vertices, uvs, normals, indices, extras);
    g_lightModelthree.Load(vertices, uvs, normals, indices, extras);
    g_lightModelthree.setupVertexAttributes();
    g_lightModelthree.setShaderID(g_shadingProg, 3);
    g_lightModelthree.setPos(glm::vec3(0.0f, 8.0f, 0.0f));
    g_lightModelthree.setScale(glm::vec3(0.04f, 0.04f, 0.04f));
    g_lightModelthree.setMaterial(g_matMatteBlackMetal);

    res = loadOBJ("models/light02_1.obj", vertices, uvs, normals, indices, extras);
    g_lightModeltable_1.Load(vertices, uvs, normals, indices, extras);
    g_lightModeltable_1.setupVertexAttributes();
    g_lightModeltable_1.setShaderID(g_shadingProg, 3);
    g_lightModeltable_1.setScale(glm::vec3(0.4f, 0.4f, 0.4f));
    g_lightModeltable_1.setPos(glm::vec3(2.0f, 2.0f, -17.0f));
    g_lightModeltable_1.setMaterial(g_matMetalGold);

    res = loadOBJ("models/light02_2.obj", vertices, uvs, normals, indices, extras);
    g_lightModeltable_2.Load(vertices, uvs, normals, indices, extras);
    g_lightModeltable_2.setupVertexAttributes();
    g_lightModeltable_2.setShaderID(g_shadingProg, 3);
    g_lightModeltable_2.setScale(glm::vec3(0.4f, 0.4f, 0.4f));
    g_lightModeltable_2.setPos(glm::vec3(2.0f, 2.0f, -17.0f));
    g_lightModeltable_2.setMaterial(g_matFabricWhiteTranslucent);

    res = loadOBJ("models/light01.obj", vertices, uvs, normals, indices, extras);
    g_lightModelone_1.Load(vertices, uvs, normals, indices, extras);
    g_lightModelone_1.setupVertexAttributes();
    g_lightModelone_1.setShaderID(g_shadingProg, 3);
    g_lightModelone_1.setScale(glm::vec3(6.0f, 6.0f, 6.0f));
    g_lightModelone_1.setRotate(-90, glm::vec3(0, 1, 0));
    g_lightModelone_1.setPos(glm::vec3(-9.0f, 5.0f, 3.5f));
    g_lightModelone_1.setMaterial(g_matMatteBlackMetal);

    g_lightModeltwo_1.Load(vertices, uvs, normals, indices, extras);
    g_lightModeltwo_1.setupVertexAttributes();
    g_lightModeltwo_1.setShaderID(g_shadingProg, 3);
    g_lightModeltwo_1.setScale(glm::vec3(6.0f, 6.0f, 6.0f));
    g_lightModeltwo_1.setRotate(-90, glm::vec3(0, 1, 0));
    g_lightModeltwo_1.setPos(glm::vec3(-9.0f, 5.0f, -4.5f));
    g_lightModeltwo_1.setMaterial(g_matMatteBlackMetal);

    res = loadOBJ("models/light01_1.obj", vertices, uvs, normals, indices, extras);
    g_lightModelone_2.Load(vertices, uvs, normals, indices, extras);
    g_lightModelone_2.setupVertexAttributes();
    g_lightModelone_2.setShaderID(g_shadingProg, 3);
    g_lightModelone_2.setScale(glm::vec3(6.0f, 6.0f, 6.0f));
    g_lightModelone_2.setRotate(-90, glm::vec3(0, 1, 0));
    g_lightModelone_2.setPos(glm::vec3(-9.0f, 5.0f, -4.5f));
    g_lightModelone_2.setMaterial(g_matGlassYellow);

    g_lightModeltwo_2.Load(vertices, uvs, normals, indices, extras);
    g_lightModeltwo_2.setupVertexAttributes();
    g_lightModeltwo_2.setShaderID(g_shadingProg, 3);
    g_lightModeltwo_2.setScale(glm::vec3(6.0f, 6.0f, 6.0f));
    g_lightModeltwo_2.setRotate(-90, glm::vec3(0, 1, 0));
    g_lightModeltwo_2.setPos(glm::vec3(-9.0f, 5.0f, 3.5f));
    g_lightModeltwo_2.setMaterial(g_matGlassYellow);
  
    //2d UI
    g_button[0].setScreenPos(400.0f, 400.0f); //準心
    g_button[0].init(g_uiShader);

    g_button[1].setScreenPos(50.0f, 60.0f);
    g_button[1].setType(1);
    g_button[1].init(g_uiShader);

    g_2dviewLoc = glGetUniformLocation(g_uiShader, "mxView"); 	// 取得 view matrix 變數位置
    glUniformMatrix4fv(g_2dviewLoc, 1, GL_FALSE, glm::value_ptr(g_2dmxView));

    g_2dProjLoc = glGetUniformLocation(g_uiShader, "mxProj"); 	// 取得 proj matrix 變數位置
    g_2dmxProj = glm::ortho(0.0f, (float)SCREEN_WIDTH, 0.0f, (float)SCREEN_HEIGHT, -1.0f, 1.0f);
    glUniformMatrix4fv(g_2dProjLoc, 1, GL_FALSE, glm::value_ptr(g_2dmxProj));

    CCamera::getInstance().updateView(g_eyeloc); // 設定 eye 位置
    CCamera::getInstance().updateCenter(glm::vec3(0, 4, 0));
    CCamera::getInstance().updatePerspective(45.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
    glm::mat4 mxView = CCamera::getInstance().getViewMatrix();
    glm::mat4 mxProj = CCamera::getInstance().getProjectionMatrix();

    GLint viewLoc = glGetUniformLocation(g_shadingProg, "mxView"); 	// 取得 view matrix 變數的位置
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mxView));

    GLint projLoc = glGetUniformLocation(g_shadingProg, "mxProj"); 	// 取得投影矩陣變數的位置
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(mxProj));

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // 設定清除 back buffer 背景的顏色
    glEnable(GL_DEPTH_TEST); // 啟動深度測試
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
//----------------------------------------------------------------------------

void render(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 設定 back buffer 的背景顏色
    glm::mat4 mxView = CCamera::getInstance().getViewMatrix();
    glm::mat4 mxProj = CCamera::getInstance().getProjectionMatrix();

    glUseProgram(g_shadingProg);
    GLint viewLoc = glGetUniformLocation(g_shadingProg, "mxView"); 	// 取得 view matrix 變數的位置
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mxView));
    GLint projLoc = glGetUniformLocation(g_shadingProg, "mxProj"); 	// 取得 mxProj 變數的位置
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(mxProj));

    //上傳光源與相機位置
    g_light[0].updateToShader();
    g_light[1].updateToShader();
    g_light[2].updateToShader();
    g_light[3].updateToShader();
    g_light[4].updateToShader();
    g_light[5].updateToShader();
    g_light[6].updateToShader();
    int lightCount = 7;
    
    for (int i = 0; i < 3; i++) {
        if (g_target[i].isBreak()) {
            g_light[i+7].updateToShader();
            lightCount++;
        }
    }
    
    glUniform1i(glGetUniformLocation(g_shadingProg, "uLightCount"), lightCount);
    glUniform3fv(glGetUniformLocation(g_shadingProg, "viewPos"), 1, glm::value_ptr(g_eyeloc));

    g_light[0].drawRaw();
    //g_light[1].drawRaw();
    g_light[2].drawRaw();
    g_light[3].drawRaw();
    g_light[4].drawRaw();
    g_light[5].drawRaw();
    g_light[6].drawRaw();
    for (int i = 0; i < 3; i++) {
        if (g_target[i].isBreak()) {
            g_light[7+i].drawRaw();
        }
    }

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 2; j++) {
            g_door[i][j].uploadMaterial();
            g_door[i][j].drawRaw();
        }
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texData[3].id);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, g_texData[45].id);
    for (int j = 0; j < 4; j++) {
        room[0].wall[j].drawRaw();
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texData[2].id);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, g_texData[46].id);
    for (int j = 0; j < 4; j++) {
        room[1].wall[j].drawRaw();
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texData[5].id);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, g_texData[31].id);
    
    for (int j = 0; j < 4; j++) {
        if (j == 1) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, g_texData[33].id);
            room[3].wall[j].drawRaw();
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        else room[3].wall[j].drawRaw();
    }
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texData[39].id);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, g_texData[40].id);
    room[2].wall[1].drawRaw();
    room[2].wall[2].drawRaw();
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);   
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_texData[32].id);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_CUBE_MAP, g_uiCubeMap2);
    room[2].wall[0].drawRaw();
    room[2].wall[3].drawRaw();
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texData[27].id);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, g_texData[28].id);
    for (int j = 0; j < 4; j++) {
        if (j == 0) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, g_texData[29].id);
            room[4].wall[j].drawRaw();
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        else room[4].wall[j].drawRaw();
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texData[21].id);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, g_texData[22].id);
    for (int j = 0; j < 4; j++) {
        room[5].wall[j].drawRaw();
    }
    for (int j = 0; j < 8; j++){
        room[0].pillair[j].uploadMaterial();
        room[0].pillair[j].drawRaw();
        room[1].pillair[j].uploadMaterial();
        room[1].pillair[j].drawRaw();
        room[2].pillair[j].uploadMaterial();
        room[2].pillair[j].drawRaw();
        room[3].pillair[j].uploadMaterial();
        room[3].pillair[j].drawRaw();
        room[4].pillair[j].uploadMaterial();
        room[4].pillair[j].drawRaw();
        room[5].pillair[j].uploadMaterial();
        room[5].pillair[j].drawRaw();
    }

    for (int i = 0; i < 6; i++) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, g_texData[8].id);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, g_texData[11].id);
        if (i == 5) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, g_texData[25].id);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, g_texData[26].id);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, g_texData[14].id);
            room[i].ground.drawRaw();
        }
        else if (i == 3) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, g_texData[34].id);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, g_texData[35].id);
            room[i].ground.drawRaw();
        }
        else if (i == 2) {
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_CUBE_MAP, g_uiCubeMap2);
            room[i].ground.drawRaw();
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        }
        else if(i != 4) room[i].ground.drawRaw();
        room[i].top.drawRaw();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texData[6].id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_texData[32].id);
    floor_outside.drawRaw();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texData[7].id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_texData[32].id);
    tvshow.drawRaw();
    
    for (int i = 0; i < 4; i++) {
        g_window[i].uploadMaterial();
        g_window[i].drawRaw();
    }   

    for (int i = 0; i < 2; i++) {
        g_window2[i].uploadMaterial();
        g_window2[i].drawRaw();
    }

    for (int i = 0; i < 1; i++) {
        g_window3[i].uploadMaterial();
        g_window3[i].drawRaw();
    }
     
    g_window4.uploadMaterial();
    g_window4.drawRaw();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texData[9].id);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, g_texData[10].id);
    for (int i = 0; i < 3; i++) {
        if (g_target[i].isBreak() == false) {
            g_target[i].drawRaw();
        }
        else if(!g_light[7+i].getMotionOn()) g_light[7 + i].setMotionEnabled();
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);

    g_lightModeltable_1.uploadMaterial();
    g_lightModeltable_1.drawRaw();

    g_vase.uploadMaterial();
    g_vase.drawRaw();

    for (int i = 0; i < 4; i++) {
        g_tree[i][0].uploadMaterial();
        g_tree[i][0].drawRaw();
        g_tree[i][1].uploadMaterial();
        g_tree[i][1].drawRaw();
    }

    g_sakula.uploadMaterial();
    g_sakula.drawRaw();

    stone.uploadMaterial();
    stone.drawRaw();

    //room-3
    g_table.uploadMaterial();
    g_table.drawRaw();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texData[30].id);
    g_black_board.drawRaw();

    g_chair.uploadMaterial();
    g_chair.drawRaw();

    g_ceilingfan.uploadMaterial();
    g_ceilingfan.drawRaw();
    g_ceilingfan_2.uploadMaterial();
    g_ceilingfan_2.drawRaw();

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_texData[32].id);
    classroom_light.uploadMaterial();
    classroom_light.drawRaw();
    
    glBindTexture(GL_TEXTURE_2D,0);

    g_lightModelone_1.uploadMaterial();
    g_lightModelone_1.drawRaw();

    g_lightModeltwo_1.uploadMaterial();
    g_lightModeltwo_1.drawRaw();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texData[38].id);
    g_gun.drawRaw();

    if (!g_key.getKeyActive()) {
        g_key.uploadMaterial();
        g_key.drawRaw();
    }
    if (g_lock[0].getLockActive()) {
        g_lock[0].uploadMaterial();
        g_lock[0].drawRaw();
        g_lock[1].uploadMaterial();
        g_lock[1].drawRaw();
    }
    if (g_lock[2].getLockActive()) {
        g_lock[2].uploadMaterial();
        g_lock[2].drawRaw();
        g_lock[3].uploadMaterial();
        g_lock[3].drawRaw();
    }
 
    for (int i = 0; i < 5; i++) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, g_texData[0].id);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, g_texData[1].id);
        g_doll[i][0].drawRaw();

        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        g_doll[i][1].uploadMaterial();
        g_doll[i][1].drawRaw();
    }

    g_sofa.uploadMaterial();
    g_sofa.drawRaw();

    g_little_table.uploadMaterial();
    g_little_table.drawRaw();
    g_tv.uploadMaterial();
    g_tv.drawRaw();

    for (int i = 0; i < 4; i++) {
        g_swing[i][0].uploadMaterial();
        g_swing[i][0].drawRaw();
        g_swing[i][1].uploadMaterial();
        g_swing[i][1].drawRaw();
    }

    g_lightModelthree.uploadMaterial();
    g_lightModelthree.drawRaw();

    g_light[6]._wing[0].uploadMaterial();
    g_light[6]._wing[0].drawRaw();


    //透明物件繪製
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            w_glass[i][j].uploadMaterial();
            w_glass[i][j].drawRaw();
        }
    }
    for (int i = 0; i < 2; i++) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, g_texData[2].id);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_CUBE_MAP, g_uiCubeMap);
        w2_glass[i].uploadMaterial();
        w2_glass[i].drawRaw();
    }

    g_lightModeltable_2.uploadMaterial();
    g_lightModeltable_2.drawRaw();

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, g_waterTex[ani].id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_texData[47].id);
    room[4].ground.uploadMaterial();
    room[4].ground.drawRaw();
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texData[17].id);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, g_texData[18].id);
    g_bird[0].drawRaw();
    g_bird[3].drawRaw();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texData[19].id);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, g_texData[20].id);
    g_bird[1].drawRaw();
    g_bird[4].drawRaw();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texData[23].id);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, g_texData[24].id);
    g_bird[2].drawRaw();
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_texData[32].id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texData[37].id);
    hint[0].drawRaw();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texData[41].id);
    hint[1].drawRaw();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texData[42].id);
    hint[2].drawRaw();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texData[43].id);
    hint[3].drawRaw();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texData[44].id);
    hint[4].drawRaw();
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texData[4].id);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, g_texData[36].id);
    for (int i = 0; i < 10; i++) {
        if (hole[i].getHoleActive()) hole[i].drawRaw();
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    g_lightModelone_2.uploadMaterial();
    g_lightModelone_2.drawRaw();

    g_lightModeltwo_2.uploadMaterial();
    g_lightModeltwo_2.drawRaw();

    for (int i = 7; i < 10; i++) {
        if (g_target[i - 7].isBreak()) {
            g_light[i]._wing[0].uploadMaterial();
            g_light[i]._wing[0].drawRaw();
            g_light[i]._wing[1].uploadMaterial();
            g_light[i]._wing[1].drawRaw();
        }
    }

    bullet_draw(mxView, mxProj);
    for (int i = 0; i < 50; i++) {
        break_cube[i].drawRaw();
    }
    for (int i = 0; i < flower_count; i++) {
        glUniform1i(glGetUniformLocation(g_nolightShader, "iColorType"), 2); // 使用 ui4Color
        glUniform4fv(glGetUniformLocation(g_nolightShader, "ui4Color"), 1, glm::value_ptr(glm::vec4(1.0f, 0.72f, 0.77f, 1.0f)));
        cflower[i].drawRaw();
    }

    glUseProgram(g_uiShader); // 使用 shader program
    glUniformMatrix4fv(g_2dviewLoc, 1, GL_FALSE, glm::value_ptr(g_2dmxView));
    glUniformMatrix4fv(g_2dProjLoc, 1, GL_FALSE, glm::value_ptr(g_2dmxProj));
    g_button[0].draw();
    if(g_key.getKeyActive()) g_button[1].draw();
}
//----------------------------------------------------------------------------

void update(float dt)
{
    g_light[0].update(dt);
    g_light[1].update(dt);
    g_light[2].update(dt);
    g_light[3].update(dt);
    g_light[4].update(dt);
    g_light[5].update(dt);
    g_light[6].update(dt);
    g_light[7].update(dt);
    g_light[8].update(dt);
    g_light[9].update(dt);

    for (int i = 0; i < 6; i++) {
        g_door[i][1].update(dt);
    }

    bullet_update(dt);
    g_doll[0][0].update(dt);
    g_doll[0][1].update(dt);
    g_target[0].update(dt);
    g_target[1].update(dt);
    g_target[2].update(dt);
    for (int i = 0; i < 50; i++) {
        break_cube[i].update(dt);
    }

    //Billboards鳥的方向永遠朝向鏡頭
    for (int i = 0; i < 5; i++) {
        glm::vec3 birdPos = g_bird[i].getPos();
        glm::vec3 dir = g_eyeloc - birdPos;

        // 只取 XZ 平面上的方向向量
        dir.y = 0.0f;
        dir = glm::normalize(dir);

        // 預設朝向（Z+）
        glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);

        // 計算旋轉角
        float angle = glm::degrees(atan2(dir.x, dir.z)); // 注意順序：x, z

        g_bird[i].setRotate(angle+180.0f, glm::vec3(0, 1, 0));  // 只繞 Y 軸旋轉
    }

    //Billboards hint
    for (int i = 0; i < 5; i++) {
        glm::vec3 hintPos = hint[i].getPos();
        glm::vec3 dir = g_eyeloc - hintPos;

        // 只取 XZ 平面上的方向向量
        dir.y = 0.0f;
        dir = glm::normalize(dir);

        // 預設朝向（Z+）
        glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);

        // 計算旋轉角
        float angle = glm::degrees(atan2(dir.x, dir.z)); // 注意順序：x, z

        hint[i].setRotate(angle + 180.0f, glm::vec3(0, 1, 0));  // 只繞 Y 軸旋轉
    }

    ani_clock += dt; //水動畫
    if (ani_clock >= 0.1f) {
        if (ani < 29) ani += 1;
        else ani = 0;
        ani_clock = 0.0f;
    }

    fan_clock += dt; //風扇轉動
    float angle_fan = 180.0f * fan_clock;
    if (angle_fan >= 360.0f) {
        fan_clock = 0.0f;
        angle_fan = 0.0f;
    }
    g_ceilingfan_2.setRotate(angle_fan, glm::vec3(0, 1, 0));
    g_gun.setRotate(angle_fan, glm::vec3(0, 1, 0));
    
    //----------------------------------------槍相關----------------------------------------
    // 取得攝影機的基底向量
    glm::vec3 camPos = g_eyeloc;
    glm::vec3 camFront = glm::normalize(CCamera::getInstance().getFront());  // 看的方向
    glm::vec3 camRight = glm::normalize(glm::cross(camFront, CCamera::getInstance().getUp()));
    glm::vec3 camUp = glm::normalize(glm::cross(camRight, camFront));

    g_gun.setRotate_center(g_eyeloc, camFront);

    // 偏移距離：靠右一點、靠下、往前
    glm::vec3 offset =
        glm::normalize(glm::cross(camFront, glm::vec3(0, 1, 0))) * 0.2f // 右
        - glm::vec3(0, 1, 0) * 0.4f                                     // 下
        + glm::normalize(glm::vec3(camFront.x, 0, camFront.z)) * 0.6f;  // 水平前
    float pitch = CCamera::getInstance().getPitch(); // -90 ~ +90
    offset.y -= glm::clamp(pitch / 90.0f, -1.0f, 1.0f) * 0.1f;

    glm::vec3 gunPos = camPos + offset;
    g_gun.setPos(glm::vec3(gunPos.x + 0.5f, gunPos.y, gunPos.z));

    //射擊
    if (shooting) {
        shooting_clock += dt;

        // 計算向後移動 + 回彈的位移（沿 z 軸為例）
        float t = shooting_clock; // 0 ~ 1 秒之間
        float back_offset = -exp(-6.0f * t) * cos(20.0f * t); // 向後猛移並震盪回來

        // 將 offset 套用到物體的位置
        glm::vec3 basePos = g_gun.getPos(); // 原始位置
        glm::vec3 offsetPos = basePos + (-glm::vec3(camFront)) * back_offset;
        g_gun.setPos(offsetPos); // 沿 z 軸移動

        if (shooting_clock >= 1.0f) {
            shooting = false;
            shooting_clock = 0.0f;

            // 回復位置
            g_gun.setPos(basePos);
        }
    }

    //----------------------------------------櫻花樹相關----------------------------------------
    if (flower_active && flower_count < 300) {
        flower_clock += dt;
        if(flower_clock >= 0.1f){
            flower_clock = 0.0f;
            flower_count += 10;
        }
    }
    if (flower_count == 300) {
        flower_clock += dt;
        int icount, icount2;
        if (flower_clock > 2.0f) { icount = 0; icount2 = 150; }
        else { icount = 150; icount2 = 0; }

        if (icount == 0) { 
            if(flower_size < 0.55) flower_size += flower_clock * 0.0005;
            else flower_size = 0.55;
            if (flower_size2 > 0.25) flower_size2 -= flower_clock * 0.0005;
            else flower_size2 = 0.25;
        }
        if (icount == 150) {
            if (flower_size > 0.25)flower_size -= flower_clock * 0.0005;
            else flower_size = 0.25;
            if (flower_size2 < 0.55)flower_size2 += flower_clock * 0.0005;
            else flower_size2 = 0.55;
        }
        for (int i = 0; i < 150; i++) {
            cflower[i].setScale(glm::vec3(flower_size, flower_size*0.1, flower_size));
        }
        for (int i = 150; i < 300; i++) {
            cflower[i].setScale(glm::vec3(flower_size2, flower_size2*0.1, flower_size2));
        }
        if (flower_clock >= 4.0f) flower_clock = 0.0f;
    }

    // 鎖掉落
    if (lockdisable) {
        lockdisable_clock += dt;

        glm::vec3 lock_pos = g_lock[0].getPos();
        glm::vec3 lock_pos2 = g_lock[1].getPos();
        g_lock[0].setPos(lock_pos - glm::vec3(0.0f, 0.2f * lockdisable_clock, 0.0f));
        g_lock[1].setPos(lock_pos2 - glm::vec3(0.0f, 0.2f * lockdisable_clock, 0.0f));

        if (lockdisable_clock > 1.0f) {
            g_lock[0].setLockActive(false);
            g_lock[1].setLockActive(false);
            lockdisable = false;
        }
    }
    if (lockdisable2) {
        lockdisable2_clock += dt;

        glm::vec3 lock_pos = g_lock[2].getPos();
        glm::vec3 lock_pos2 = g_lock[3].getPos();
        g_lock[2].setPos(lock_pos - glm::vec3(0.0f, 0.2f * lockdisable2_clock, 0.0f));
        g_lock[3].setPos(lock_pos2 - glm::vec3(0.0f, 0.2f * lockdisable2_clock, 0.0f));

        if (lockdisable2_clock > 1.0f) {
            g_lock[2].setLockActive(false);
            g_lock[3].setLockActive(false);
            lockdisable2 = false;
        }
    }

    //槍擊孔洞
    for (int i = 0; i < 10; i++) {
        hole[i].update(dt);
    }
}

void releaseAll()
{

}

int main() {
    // ------- 檢查與建立視窗  ---------------  
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // 設定 OpenGL 版本與 Core Profile
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //只啟用 OpenGL 3.3 Core Profile（不包含舊版 OpenGL 功能）
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // 禁止視窗大小改變

    // 建立 OpenGL 視窗與該視窗執行時所需的的狀態、資源和環境(context 上下文)
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL_4 Example 4 NPR", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // 設定將這個視窗的資源(OpenGL 的圖形上下文）與當前執行緒綁定，讓該執行緒能夠操作該視窗的資源
    glfwMakeContextCurrent(window);

    // 設定視窗大小, 這樣 OpenGL 才能知道如何將視窗的內容繪製到正確的位置
    // 基本上寬與高設定成視窗的寬與高即可
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    // ---------------------------------------

    // 設定相關事件的 callback 函式，以便事件發生時，能呼叫對應的函式
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);// 視窗大小被改變時
    glfwSetKeyCallback(window, keyCallback);                        // 有鍵盤的按鍵被按下時
    glfwSetMouseButtonCallback(window, mouseButtonCallback);        // 有滑鼠的按鍵被按下時
    glfwSetCursorPosCallback(window, cursorPosCallback);            // 滑鼠在指定的視窗上面移動時
    glfwSetScrollCallback(window, scrollCallback);			        // 滑鼠滾輪滾動時

    // 呼叫 loadScene() 建立與載入 GPU 進行描繪的幾何資料 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    loadScene();

    float lastTime = (float)glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - lastTime; // 計算前一個 frame 到目前為止經過的時間
        lastTime = currentTime;
        update(deltaTime);      // 呼叫 update 函式，並將 deltaTime 傳入，讓所有動態物件根據時間更新相關內容
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    releaseAll(); // 程式結束前釋放所有的資源
    glfwTerminate();
    return 0;
}

void genMaterial()
{
    // 設定材質
    g_matBeige.setAmbient(glm::vec4(0.0918f, 0.0906f, 0.0863f, 1.0f));
    g_matBeige.setDiffuse(glm::vec4(0.8258f, 0.8152f, 0.7765f, 1.0f));
    g_matBeige.setSpecular(glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));
    g_matBeige.setShininess(32.0f);

    g_matWaterGreen.setAmbient(glm::vec4(0.075f, 0.120f, 0.090f, 1.0f));
    g_matWaterGreen.setDiffuse(glm::vec4(0.540f, 0.840f, 0.640f, 1.0f));
    g_matWaterGreen.setSpecular(glm::vec4(0.20f, 0.20f, 0.20f, 1.0f));
    g_matWaterGreen.setShininess(32.0f);

    g_matWoodHoney.setAmbient(glm::vec4(0.180f, 0.164f, 0.130f, 1.0f));
    g_matWoodHoney.setDiffuse(glm::vec4(0.720f, 0.656f, 0.520f, 1.0f));
    g_matWoodHoney.setSpecular(glm::vec4(0.30f, 0.30f, 0.30f, 1.0f));
    g_matWoodHoney.setShininess(24.0f);

    g_matWoodBleached.setAmbient(glm::vec4(0.220f, 0.215f, 0.205f, 1.0f));
    g_matWoodBleached.setDiffuse(glm::vec4(0.880f, 0.860f, 0.820f, 1.0f));
    g_matWoodBleached.setSpecular(glm::vec4(0.30f, 0.30f, 0.30f, 1.0f));
    g_matWoodBleached.setShininess(24.0f);

    g_matGlassYellow.setAmbient(glm::vec4(0.2f, 0.18f, 0.05f, 0.3f));
    g_matGlassYellow.setDiffuse(glm::vec4(0.9f, 0.85f, 0.3f, 0.3f));
    g_matGlassYellow.setSpecular(glm::vec4(0.8f, 0.8f, 0.6f, 0.3f));
    g_matGlassYellow.setShininess(96.0f);

    g_matGlassPink.setAmbient(glm::vec4(0.4f, 0.15f, 0.3f, 0.3f));   // 淡粉紅、微暗
    g_matGlassPink.setDiffuse(glm::vec4(1.0f, 0.3f, 0.6f, 0.3f));   // 明亮粉紅光
    g_matGlassPink.setSpecular(glm::vec4(1.0f, 0.6f, 0.9f, 0.3f));  // 帶點白亮高光
    g_matGlassPink.setShininess(96.0f);

    g_matGlassSkyBlue.setAmbient(glm::vec4(0.2f, 0.4f, 0.6f, 0.3f));   // 微暗藍綠
    g_matGlassSkyBlue.setDiffuse(glm::vec4(0.3f, 0.9f, 1.0f, 0.3f));   // 天藍主色
    g_matGlassSkyBlue.setSpecular(glm::vec4(0.9f, 1.0f, 1.0f, 0.3f));  // 明亮高光
    g_matGlassSkyBlue.setShininess(96.0f);

    g_matGlassLightGreen.setAmbient(glm::vec4(0.15f, 0.35f, 0.1f, 0.3f));   // 柔和綠底
    g_matGlassLightGreen.setDiffuse(glm::vec4(0.7f, 1.0f, 0.3f, 0.3f));    // 明亮嫩綠
    g_matGlassLightGreen.setSpecular(glm::vec4(0.9f, 1.0f, 0.6f, 0.3f));   // 帶一點白的高光
    g_matGlassLightGreen.setShininess(96.0f);

    g_matPlantGreen.setAmbient(glm::vec4(0.1f, 0.25f, 0.1f, 1.0f));
    g_matPlantGreen.setDiffuse(glm::vec4(0.3f, 0.6f, 0.3f, 1.0f));
    g_matPlantGreen.setSpecular(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    g_matPlantGreen.setShininess(8.0f);

    g_matPaper.setAmbient(glm::vec4(0.25f, 0.25f, 0.22f, 1.0f));
    g_matPaper.setDiffuse(glm::vec4(0.95f, 0.95f, 0.9f, 1.0f));
    g_matPaper.setSpecular(glm::vec4(0.05f, 0.05f, 0.05f, 1.0f));
    g_matPaper.setShininess(8.0f);

    g_matPotBrickRed.setAmbient(glm::vec4(0.25f, 0.10f, 0.05f, 1.0f));
    g_matPotBrickRed.setDiffuse(glm::vec4(0.6f, 0.2f, 0.1f, 1.0f));
    g_matPotBrickRed.setSpecular(glm::vec4(0.15f, 0.1f, 0.1f, 1.0f));
    g_matPotBrickRed.setShininess(12.0f);

    g_matFabricWhiteTranslucent.setAmbient(glm::vec4(0.5f, 0.5f, 0.5f, 0.7f));
    g_matFabricWhiteTranslucent.setDiffuse(glm::vec4(0.9f, 0.9f, 0.9f, 0.7f));
    g_matFabricWhiteTranslucent.setSpecular(glm::vec4(0.3f, 0.3f, 0.3f, 0.7f));
    g_matFabricWhiteTranslucent.setShininess(32.0f);

    g_matMetalGold.setAmbient(glm::vec4(0.247f, 0.199f, 0.074f, 1.0f));
    g_matMetalGold.setDiffuse(glm::vec4(0.751f, 0.606f, 0.226f, 1.0f));
    g_matMetalGold.setSpecular(glm::vec4(0.628f, 0.555f, 0.366f, 1.0f));
    g_matMetalGold.setShininess(64.0f);

    g_matMatteBlackMetal.setAmbient(glm::vec4(0.02f, 0.02f, 0.02f, 1.0f));
    g_matMatteBlackMetal.setDiffuse(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    g_matMatteBlackMetal.setSpecular(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    g_matMatteBlackMetal.setShininess(16.0f);

    g_matClothBeige.setAmbient(glm::vec4(0.1f, 0.1f, 0.08f, 1.0f));
    g_matClothBeige.setDiffuse(glm::vec4(0.6f, 0.6f, 0.5f, 1.0f));
    g_matClothBeige.setSpecular(glm::vec4(0.05f, 0.05f, 0.05f, 1.0f));
    g_matClothBeige.setShininess(10.0f);

    g_matWoodWhite.setAmbient(glm::vec4(0.2f, 0.2f, 0.18f, 1.0f));   // 微暖白光，陰影中不死白
    g_matWoodWhite.setDiffuse(glm::vec4(0.9f, 0.9f, 0.85f, 1.0f));   // 主體白中帶自然感
    g_matWoodWhite.setSpecular(glm::vec4(0.05f, 0.05f, 0.05f, 1.0f)); // 木材幾乎無反光
    g_matWoodWhite.setShininess(8.0f);

    g_matConcreteWhite.setAmbient(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));     // 陰影區仍保有些許亮度
    g_matConcreteWhite.setDiffuse(glm::vec4(0.95f, 0.95f, 0.95f, 1.0f)); // 主體白色
    g_matConcreteWhite.setSpecular(glm::vec4(0.05f, 0.05f, 0.05f, 1.0f)); // 幾乎無光澤
    g_matConcreteWhite.setShininess(4.0f);

    g_matWoodDark.setAmbient(glm::vec4(0.08f, 0.05f, 0.03f, 1.0f));   // 深色環境色
    g_matWoodDark.setDiffuse(glm::vec4(0.25f, 0.15f, 0.10f, 1.0f));   // 深紅棕色調
    g_matWoodDark.setSpecular(glm::vec4(0.10f, 0.08f, 0.06f, 1.0f));  // 柔和鏡面光
    g_matWoodDark.setShininess(16.0f);                                // 微光澤感

    g_matWater.setAmbient(glm::vec4(0.0f, 0.1f, 0.3f, 0.4f));     // 深藍，環境色低調
    g_matWater.setDiffuse(glm::vec4(0.0f, 0.6f, 1.0f, 0.4f));     // 飽和藍綠，主要顏色
    g_matWater.setSpecular(glm::vec4(0.8f, 0.9f, 1.0f, 0.4f));    // 明亮高光
    g_matWater.setShininess(96.0f);                              // 亮澤感強烈

    g_matYellowSofa.setAmbient(glm::vec4(0.3f, 0.3f, 0.1f, 1.0f));    // 柔和黃褐環境色
    g_matYellowSofa.setDiffuse(glm::vec4(1.0f, 0.85f, 0.2f, 1.0f));   // 明亮溫暖的黃
    g_matYellowSofa.setSpecular(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));   // 低調高光，不會太塑膠
    g_matYellowSofa.setShininess(32.0f);  // 適中光澤，偏向布料或亞光皮革
}