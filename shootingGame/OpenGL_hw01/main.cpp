#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <fstream>
#include <sstream>

#include <glew/include/GL/glew.h>
#include <glfw/include/GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>
#include <time.h>
//#include <glm/gtx/string_cast.hpp>

#include "common/initshader.h"
#include "common/arcball.h"
#include "common/wmhandler.h"
#include "common/CShaderPool.h"
#include "common/CBulletTwo.h"
#include "common/CAirplane.h"
#include "common/CBulletOne.h"
#include "common/bullet.h"
#include "common/CShield.h"
#include "common/background.h"
#include "common/villain.h"
#include "common/skill.h"
#include "common/CLevel.h"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 800 

Arcball g_arcball;
bool g_bRotating = false;
bool g_bMoving = false;

GLuint g_shaderProg;
glm::mat4 g_viewMx = glm::mat4(1.0f);
glm::mat4 g_projMx = glm::mat4(1.0f);
GLfloat g_viewScale = 4.0f;

CLevel header;
CAirplane g_airplane;
CShield player_shield;
int re_shield;
glm::mat4  mxshield_Dist[3]; 
glm::vec3  shield_Dist[3] = {
    glm::vec3(0.2f, 0.0f, 0.0f),  
    glm::vec3(-0.1f, 0.1732f, 0.0f),
    glm::vec3(-0.1f, -0.1732f, 0.0f) 
};

//----------------------------------------------------------------------------
void loadScene(void)
{
    // getShader 函式可用於建立 shader program 或取得特定 shader program 的代表 ID 
    g_shaderProg = CShaderPool::instance().getShader("vshader21.glsl", "fshader21.glsl");

    g_airplane.setupVertexAttributes();
    g_airplane.setShaderID(g_shaderProg);
    g_airplane.setPos(glm::vec3(0.0f, -3.0f, 0.0f));
    for (int i = 0; i < 6; i++) {
        g_airplane.blood[i].setupVertexAttributes();
        g_airplane.blood[i].setShaderID(g_shaderProg);
        g_airplane.setPos(glm::vec3(-4.0f, -4.0f, 0.0f));
    }
    g_airplane.dead.setupVertexAttributes();
    g_airplane.dead.setShaderID(g_shaderProg);

    player_shield.setupVertexAttributes();
    player_shield.setShaderID(g_shaderProg);
    player_shield.setPos(g_airplane.getPos());

    header.setupVertexAttributes();
    header.setShaderID(g_shaderProg);
    header.setPos(glm::vec3(0.0f));
    
    for (int i = 0; i < player_shield.getBlood(); i++) {
        player_shield.blood[i].setupVertexAttributes();
        player_shield.blood[i].setShaderID(g_shaderProg);
    }

    re_shield = player_shield.getStatus();
    
    Villain();
    skill();

    GLint viewLoc = glGetUniformLocation(g_shaderProg, "mxView"); 	// 取得 MVP 變數的位置
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(g_viewMx));

    g_projMx = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, -2.0f, 2.0f);
    GLint projLoc = glGetUniformLocation(g_shaderProg, "mxProj"); 	// 取得 MVP 變數的位置
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(g_projMx));
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 設定清除 back buffer 背景的顏色
}
//----------------------------------------------------------------------------

void render(void)
{
    glClear(GL_COLOR_BUFFER_BIT);			// clear the window
    background_draw();
    bullet_draw();
    if (g_airplane.getStatus() != 0) {
        if (player_shield.getStatus() != 0) player_shield.draw();
        g_airplane.draw();
    }
    v_draw();
    skill_draw();
    header.draw();
}
//----------------------------------------------------------------------------

float g_angle = 0.0f;
void update(float dt)
{
    //g_airplane.bullet_collision(g_villain);
    bullet_collision();
    v_update(dt);
    background_update(dt);

    if (g_airplane.getStatus() != 0) g_airplane.update(dt);
    else if(header.getType() != 0) header.HeaderOn(0);

    player_shield.update(dt);
    if (re_shield != player_shield.getStatus()) {
        re_shield = player_shield.getStatus();
    }
    if (player_shield.getStatus() == 2) {
        player_shield.setPos(g_airplane.getPos());
    }
    if (player_shield.getStatus() == 1) { //護盾繞飛機旋轉
        float radius = 0.6f;
        g_angle += 90.0f * dt;
        if (g_angle > 360.0f) g_angle -= 360.0f;
        player_shield.setPos(glm::vec3(0.0f));

        glm::vec3 offset = glm::vec3(radius, radius, 0);

        glm::vec4 rotated_offset = glm::rotate( // 計算旋轉後的位置
            glm::mat4(1.0f),
            glm::radians(g_angle),
            glm::vec3(0.0f, 0.0f, 1.0f)
        ) * glm::vec4(offset, 1.0f);

        // 設定物件位置為飛機位置 + 繞圈後的 offset
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), g_airplane.getPos() + glm::vec3(rotated_offset));
        player_shield.setTransformMatrix(transform);

        //血量繞護盾旋轉
        float blood_radius = 0.02f; // 血量與護盾的距離

        for (int i = 0; i < 3; i++) {
            glm::mat4 blood_transform =
                glm::translate(glm::mat4(1.0f), player_shield.getPos()) *
                glm::rotate(glm::mat4(1.0f), glm::radians(g_angle*2.0f), glm::vec3(0, 0, 1.0f)) *
                glm::translate(glm::mat4(1.0f), glm::vec3(blood_radius, blood_radius, 0)); // 沿 X 軸展開

            mxshield_Dist[i] = glm::translate(glm::mat4(1.0f), shield_Dist[i]);
            player_shield.blood[i].setTransformMatrix(transform * blood_transform * mxshield_Dist[i]);
        }
    }
    

    bullet_update(dt);
    skill_update(dt);
    header.update(dt);
}

void releaseAll()
{
    bullet_release();
    v_release();
    background_release();
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
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Draw ", nullptr, nullptr);
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

    // 呼叫 loadScene() 建立與載入 GPU 進行描繪的幾何資料 
    loadScene();
    srand(time(NULL));

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