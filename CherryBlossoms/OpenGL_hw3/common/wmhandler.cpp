#include <iostream>
#include <glew/include/GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <array>

#include "CCamera.h"
#include "wmhandler.h"
#include "arcball.h"

#include "../common/CMaterial.h"
#include "../models/CQuad.h"
#include "../models/CCube.h"
#include "../common/CLight.h"
#include "CButton.h"
#include "../models/CObj.h"
#include "../common/bullet.h"
#include "../models/CRoom.h"

//#define SPOT_TARGET  // Example 2 解開

extern Arcball g_arcball;
extern CObj g_door[6][2];

bool  g_bCamRoting = false; // 用於鏡頭的旋轉
bool  g_bfirstMouse = true;   // 滑鼠左鍵首次按下，預設為 true
float g_lastX = 400, g_lastY = 400; // 滑鼠移動的距離
float g_mouseSens = 0.5f;   // 位移鏡頭的靈敏度  
glm::vec3 direction;
float moveDistance = 0.3f; //wasd位移靈敏度
float newx;
float newy;
float newz;

float lastxpos;
float lastypos;

float r = 1.0f, g = 0.9f, b = 0.5f;

int lightLevel = 2;

extern CCube g_centerloc;
extern GLuint g_shadingProg;
extern glm::vec3 g_eyeloc;
extern CLight g_light[10];
extern std::array<CButton, 5> g_button;
extern CRoom room[6];
extern CObj g_gun, g_key, g_sakula;

extern CMaterial g_matWaterGreen;
extern CQuad g_bird[6];

extern bool shooting;
extern float shooting_clock;
extern bool lockdisable;
extern bool lockdisable2;
extern bool flower_active;

Arcball g_arcball; //保留未用

// 滑鼠按鈕按下後 callback function(回呼函式) ---------------
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    // Arcball* arcball = static_cast<Arcball*>(glfwGetWindowUserPointer(window));
    g_arcball.onMouseButton(button, action, xpos, ypos);
    //std::cout << "button = " << button << "action = " << action << "mods = " << mods << std::endl;
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
        {
            if (!shooting) {
                std::cout << "button right" << std::endl;
                glm::vec3 eyePos = CCamera::getInstance().getViewLocation();
                glm::vec3 front = CCamera::getInstance().getFront();
                shoot(eyePos, front);
                shooting = true;
                shooting_clock = 0.0f;
            }
        }
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            g_bfirstMouse = true;
            g_bCamRoting = !g_bCamRoting;
        }
    }
}
// ---------------------------------------------------------------------------------------

// 滑鼠在指定視窗上移動時的 callback function (回呼函式)
void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    g_arcball.onCursorMove(xpos, ypos, width, height);
    // Arcball* arcball = static_cast<Arcball*>(glfwGetWindowUserPointer(window));
    //std::cout << "x = " << xpos << "y = " << ypos << std::endl;
    if (g_bCamRoting)  // 進入控制鏡頭模式
    {
        if (g_bfirstMouse) {
            g_lastX = xpos;
            g_lastY = ypos;
            g_bfirstMouse = false;
            return;
        }
        float deltaX = ((float)xpos - g_lastX);
        float deltaY = (g_lastY - (float)ypos);
        g_lastX = (float)xpos; g_lastY = (float)ypos;

        glm::vec3 eyePos = CCamera::getInstance().getViewLocation();

        CCamera::getInstance().processMouseMovement2(deltaX, deltaY, g_mouseSens);
        glm::mat4 mxView = CCamera::getInstance().getViewMatrix();
        GLint viewLoc = glGetUniformLocation(g_shadingProg, "mxView"); 	// 取得 view matrix 變數位置
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mxView));
        g_eyeloc = CCamera::getInstance().getViewLocation();
    }
}
// ---------------------------------------------------------------------------------------
// 當視窗大小改變的 callback function
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// ---------------------------------------------------------------------------------------
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    
}

// 當鍵盤按鍵按下時的 callback function (回呼函式)
// key : GLFW_KEY_0、 GLFW_KEY_a、GLFW_KEY_ESCAPE、GLFW_KEY_SPACE
//       完整定義在這邊 https://www.glfw.org/docs/latest/group__keys.html
// action : 
//          GLFW_PRESS：按鍵被按下。
//          GLFW_RELEASE：按鍵被釋放。
//          GLFW_REPEAT：按鍵被重複觸發（長按時會觸發多次）
// mods : 
//          GLFW_MOD_SHIFT：Shift 鍵被按下。
//          GLFW_MOD_CONTROL：Ctrl 鍵被按下。
//          GLFW_MOD_ALT：Alt 鍵被按下。
//          GLFW_MOD_SUPER：Windows 鍵或 Command 鍵被按下。
//          GLFW_MOD_CAPS_LOCK：Caps Lock 鍵被啟用。
//          GLFW_MOD_NUM_LOCK：Num Lock 鍵被啟用。
// 
//  注意：1. 一個按鍵按下到放開會產生兩次 keyCallback，
//           根據功能需求判斷是按下或是放掉去執行對應的功能
//        2. 使用 glfw 英文字母沒有分大小寫，都是傳回大寫的 ASCII 編碼
//           搭配檢查左右 shift 鍵是否按下是大寫還是小寫(假設 caps 鍵沒有被按下)
//       
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    glm::vec3 vPos;
    glm::mat4 mxView, mxProj;
    GLint viewLoc, projLoc;
    float shin;

    switch (key)
    {
    case GLFW_KEY_ESCAPE:
        if (action == GLFW_PRESS) { glfwSetWindowShouldClose(window, true); }
        break;
    case GLFW_KEY_SPACE:
        break;

    default: // 針對英文字母大小寫進行處理
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            // 檢查 Shift 鍵(左右兩邊各一個)是否被按下
            bool isShiftPressed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ||
                (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);
            // 判斷字母鍵的大小寫，預設使用鍵盤是沒有按下 CAPS 鍵
            if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
                char letter = (isShiftPressed) ? ('A' + (key - GLFW_KEY_A)) : ('a' + (key - GLFW_KEY_A));
                switch (letter) {
                case 'r':
                case 'R':
                    r += 0.1f;
                    if (r > 1.0f) r = 0.0f;
                    g_light[3].setDiffuse(glm::vec4(r, g, b, 1.0f));
                    break;
                case 'g':
                case 'G':
                    g += 0.1f;
                    if (g > 1.0f) g = 0.0f;
                    g_light[3].setDiffuse(glm::vec4(r, g, b, 1.0f));
                    break;
                case 'b':
                case 'B':
                    b += 0.1f;
                    if (b > 1.0f) b = 0.0f;
                    g_light[3].setDiffuse(glm::vec4(r, g, b, 1.0f));
                    break;
                case 'W':
                case 'w':
                    g_eyeloc = CCamera::getInstance().getViewLocation(); // 取的 eye 位置

                    direction = CCamera::getInstance().getFront();

                    newx = g_eyeloc.x + direction.x * moveDistance;
                    newz = g_eyeloc.z + direction.z * moveDistance;

                    if (newx >= -9.0f) {
                        if (newz >= -9.0f) {
                            if (room[0].Collisionx(newx, newz, g_door[4][1].getDoorOpen(), g_door[4][0].getPos().x, g_door[4][0].getPos().z, 0.04f) == 1) newx = g_eyeloc.x;
                            if (room[0].Collisionz(newx, newz, g_door[0][1].getDoorOpen(), g_door[0][0].getPos().x, g_door[0][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                        }
                        else if (newz >= -27.01f) {
                            if (room[1].Collisionx(newx, newz, 0, 0, 0, 0.04f) == 1) newx = g_eyeloc.x;
                            if (newz > -18.0f) {
                                if (room[1].Collisionz(newx, newz, g_door[0][1].getDoorOpen(), g_door[0][0].getPos().x, g_door[0][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                            }
                            else {
                                if (room[1].Collisionz(newx, newz, g_door[1][1].getDoorOpen(), g_door[1][0].getPos().x, g_door[1][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                            }
                        }
                        else {
                            if (room[2].Collisionx(newx, newz, g_door[5][1].getDoorOpen(), g_door[5][0].getPos().x, g_door[5][0].getPos().z, 0.04f) == 1) newx = g_eyeloc.x;
                            if (room[2].Collisionz(newx, newz, g_door[1][1].getDoorOpen(), g_door[1][0].getPos().x, g_door[1][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                        }   
                    }
                    else {
                        if (newz >= -9.0f) {
                            if (room[3].Collisionx(newx, newz, g_door[4][1].getDoorOpen(), g_door[4][0].getPos().x, g_door[4][0].getPos().z, 0.04f) == 1) newx = g_eyeloc.x;
                            if (room[3].Collisionz(newx, newz, g_door[3][1].getDoorOpen(), g_door[3][0].getPos().x, g_door[3][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                        }
                        else if (newz >= -27.01f) {
                            if (room[4].Collisionx(newx, newz, 0, 0, 0, 0.04f) == 1) newx = g_eyeloc.x;
                            if (newz > -18.0f) {
                                if (room[4].Collisionz(newx, newz, g_door[3][1].getDoorOpen(), g_door[3][0].getPos().x, g_door[3][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                            }
                            else {
                                if (room[4].Collisionz(newx, newz, g_door[2][1].getDoorOpen(), g_door[2][0].getPos().x, g_door[2][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                            }
                        }
                        else {
                            if (room[5].Collisionx(newx, newz, g_door[5][1].getDoorOpen(), g_door[5][0].getPos().x, g_door[5][0].getPos().z, 0.04f) == 1) newx = g_eyeloc.x;
                            if (room[5].Collisionz(newx, newz, g_door[2][1].getDoorOpen(), g_door[2][0].getPos().x, g_door[2][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                        }  
                    }

                    if (g_eyeloc.x != newx) g_eyeloc.x += direction.x * moveDistance; // eye 同步移動
                    if (g_eyeloc.z != newz) g_eyeloc.z += direction.z * moveDistance;

                    CCamera::getInstance().move(g_eyeloc);
                    mxView = CCamera::getInstance().getViewMatrix();
                    viewLoc = glGetUniformLocation(g_shadingProg, "mxView"); // 取得 view matrix 變數的位置
                    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mxView));
                    break;
                case 'S':
                case 's':
                    g_eyeloc = CCamera::getInstance().getViewLocation(); // 取的 eye 位置

                    direction = CCamera::getInstance().getFront();

                    newx = g_eyeloc.x - direction.x * moveDistance;
                    newz = g_eyeloc.z - direction.z * moveDistance;
                    if (newx >= -9.0f) {
                        if (newz >= -9.0f) {
                            if (room[0].Collisionx(newx, newz, g_door[4][1].getDoorOpen(), g_door[4][0].getPos().x, g_door[4][0].getPos().z, 0.04f) == 1) newx = g_eyeloc.x;
                            if (room[0].Collisionz(newx, newz, g_door[0][1].getDoorOpen(), g_door[0][0].getPos().x, g_door[0][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                        }
                        else if (newz >= -27.01f) {
                            if (room[1].Collisionx(newx, newz, 0, 0, 0, 0.04f) == 1) newx = g_eyeloc.x;
                            if (newz > -18.0f) {
                                if (room[1].Collisionz(newx, newz, g_door[0][1].getDoorOpen(), g_door[0][0].getPos().x, g_door[0][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                            }
                            else {
                                if (room[1].Collisionz(newx, newz, g_door[1][1].getDoorOpen(), g_door[1][0].getPos().x, g_door[1][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                            }
                        }
                        else {
                            if (room[2].Collisionx(newx, newz, g_door[5][1].getDoorOpen(), g_door[5][0].getPos().x, g_door[5][0].getPos().z, 0.04f) == 1) newx = g_eyeloc.x;
                            if (room[2].Collisionz(newx, newz, g_door[1][1].getDoorOpen(), g_door[1][0].getPos().x, g_door[1][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                        }
                    }
                    else {
                        if (newz >= -9.0f) {
                            if (room[3].Collisionx(newx, newz, g_door[4][1].getDoorOpen(), g_door[4][0].getPos().x, g_door[4][0].getPos().z, 0.04f) == 1) newx = g_eyeloc.x;
                            if (room[3].Collisionz(newx, newz, g_door[3][1].getDoorOpen(), g_door[3][0].getPos().x, g_door[3][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                        }
                        else if (newz >= -27.01f) {
                            if (room[4].Collisionx(newx, newz, 0, 0, 0, 0.04f) == 1) newx = g_eyeloc.x;
                            if (newz > -18.0f) {
                                if (room[4].Collisionz(newx, newz, g_door[3][1].getDoorOpen(), g_door[3][0].getPos().x, g_door[3][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                            }
                            else {
                                if (room[4].Collisionz(newx, newz, g_door[2][1].getDoorOpen(), g_door[2][0].getPos().x, g_door[2][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                            }
                        }
                        else {
                            if (room[5].Collisionx(newx, newz, g_door[5][1].getDoorOpen(), g_door[5][0].getPos().x, g_door[5][0].getPos().z, 0.04f) == 1) newx = g_eyeloc.x;
                            if (room[5].Collisionz(newx, newz, g_door[2][1].getDoorOpen(), g_door[2][0].getPos().x, g_door[2][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                        }
                    }

                    if (g_eyeloc.x != newx) g_eyeloc.x -= direction.x * moveDistance; // eye 同步移動
                    if (g_eyeloc.z != newz) g_eyeloc.z -= direction.z * moveDistance;

                    CCamera::getInstance().move(g_eyeloc);
                    mxView = CCamera::getInstance().getViewMatrix();
                    viewLoc = glGetUniformLocation(g_shadingProg, "mxView"); // 取得 view matrix 變數的位置
                    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mxView));
                    break;
                case 'A':
                case 'a':
                    g_eyeloc = CCamera::getInstance().getViewLocation(); // 取的 eye 位置

                    direction = CCamera::getInstance().getFront();
                    direction = glm::vec3(direction.z, 0.0f, -direction.x); //向左轉

                    newx = g_eyeloc.x + direction.x * moveDistance;
                    newz = g_eyeloc.z + direction.z * moveDistance;
                    if (newx >= -9.0f) {
                        if (newz >= -9.0f) {
                            if (room[0].Collisionx(newx, newz, g_door[4][1].getDoorOpen(), g_door[4][0].getPos().x, g_door[4][0].getPos().z, 0.04f) == 1) newx = g_eyeloc.x;
                            if (room[0].Collisionz(newx, newz, g_door[0][1].getDoorOpen(), g_door[0][0].getPos().x, g_door[0][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                        }
                        else if (newz >= -27.01f) {
                            if (room[1].Collisionx(newx, newz, 0, 0, 0, 0.04f) == 1) newx = g_eyeloc.x;
                            if (newz > -18.0f) {
                                if (room[1].Collisionz(newx, newz, g_door[0][1].getDoorOpen(), g_door[0][0].getPos().x, g_door[0][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                            }
                            else {
                                if (room[1].Collisionz(newx, newz, g_door[1][1].getDoorOpen(), g_door[1][0].getPos().x, g_door[1][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                            }
                        }
                        else {
                            if (room[2].Collisionx(newx, newz, g_door[5][1].getDoorOpen(), g_door[5][0].getPos().x, g_door[5][0].getPos().z, 0.04f) == 1) newx = g_eyeloc.x;
                            if (room[2].Collisionz(newx, newz, g_door[1][1].getDoorOpen(), g_door[1][0].getPos().x, g_door[1][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                        }
                    }
                    else {
                        if (newz >= -9.0f) {
                            if (room[3].Collisionx(newx, newz, g_door[4][1].getDoorOpen(), g_door[4][0].getPos().x, g_door[4][0].getPos().z, 0.04f) == 1) newx = g_eyeloc.x;
                            if (room[3].Collisionz(newx, newz, g_door[3][1].getDoorOpen(), g_door[3][0].getPos().x, g_door[3][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                        }
                        else if (newz >= -27.01f) {
                            if (room[4].Collisionx(newx, newz, 0, 0, 0, 0.04f) == 1) newx = g_eyeloc.x;
                            if (newz > -18.0f) {
                                if (room[4].Collisionz(newx, newz, g_door[3][1].getDoorOpen(), g_door[3][0].getPos().x, g_door[3][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                            }
                            else {
                                if (room[4].Collisionz(newx, newz, g_door[2][1].getDoorOpen(), g_door[2][0].getPos().x, g_door[2][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                            }
                        }
                        else {
                            if (room[5].Collisionx(newx, newz, g_door[5][1].getDoorOpen(), g_door[5][0].getPos().x, g_door[5][0].getPos().z, 0.04f) == 1) newx = g_eyeloc.x;
                            if (room[5].Collisionz(newx, newz, g_door[2][1].getDoorOpen(), g_door[2][0].getPos().x, g_door[2][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                        }
                    }

                    if (g_eyeloc.x != newx) g_eyeloc.x += direction.x * moveDistance; // eye 同步移動
                    if (g_eyeloc.z != newz) g_eyeloc.z += direction.z * moveDistance;
                    
                    CCamera::getInstance().move(g_eyeloc);
                    mxView = CCamera::getInstance().getViewMatrix();
                    viewLoc = glGetUniformLocation(g_shadingProg, "mxView"); // 取得 view matrix 變數的位置
                    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mxView));
                    break;
                case 'D':
                case 'd':
                    g_eyeloc = CCamera::getInstance().getViewLocation(); // 取的 eye 位置

                    direction = CCamera::getInstance().getFront();
                    direction = glm::vec3(-direction.z, 0.0f, direction.x); //向右轉

                    newx = g_eyeloc.x + direction.x * moveDistance;
                    newz = g_eyeloc.z + direction.z * moveDistance;
                    if (newx >= -9.0f) {
                        if (newz >= -9.0f) {
                            if (room[0].Collisionx(newx, newz, g_door[4][1].getDoorOpen(), g_door[4][0].getPos().x, g_door[4][0].getPos().z, 0.04f) == 1) newx = g_eyeloc.x;
                            if (room[0].Collisionz(newx, newz, g_door[0][1].getDoorOpen(), g_door[0][0].getPos().x, g_door[0][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                        }
                        else if (newz >= -27.01f) {
                            if (room[1].Collisionx(newx, newz, 0, 0, 0, 0.04f) == 1) newx = g_eyeloc.x;
                            if (newz > -18.0f) {
                                if (room[1].Collisionz(newx, newz, g_door[0][1].getDoorOpen(), g_door[0][0].getPos().x, g_door[0][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                            }
                            else {
                                if (room[1].Collisionz(newx, newz, g_door[1][1].getDoorOpen(), g_door[1][0].getPos().x, g_door[1][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                            }
                        }
                        else {
                            if (room[2].Collisionx(newx, newz, g_door[5][1].getDoorOpen(), g_door[5][0].getPos().x, g_door[5][0].getPos().z, 0.04f) == 1) newx = g_eyeloc.x;
                            if (room[2].Collisionz(newx, newz, g_door[1][1].getDoorOpen(), g_door[1][0].getPos().x, g_door[1][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                        }
                    }
                    else {
                        if (newz >= -9.0f) {
                            if (room[3].Collisionx(newx, newz, g_door[4][1].getDoorOpen(), g_door[4][0].getPos().x, g_door[4][0].getPos().z, 0.04f) == 1) newx = g_eyeloc.x;
                            if (room[3].Collisionz(newx, newz, g_door[3][1].getDoorOpen(), g_door[3][0].getPos().x, g_door[3][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                        }
                        else if (newz >= -27.01f) {
                            if (room[4].Collisionx(newx, newz, 0, 0, 0, 0.04f) == 1) newx = g_eyeloc.x;
                            if (newz > -18.0f) {
                                if (room[4].Collisionz(newx, newz, g_door[3][1].getDoorOpen(), g_door[3][0].getPos().x, g_door[3][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                            }
                            else {
                                if (room[4].Collisionz(newx, newz, g_door[2][1].getDoorOpen(), g_door[2][0].getPos().x, g_door[2][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                            }
                        }
                        else {
                            if (room[5].Collisionx(newx, newz, g_door[5][1].getDoorOpen(), g_door[5][0].getPos().x, g_door[5][0].getPos().z, 0.04f) == 1) newx = g_eyeloc.x;
                            if (room[5].Collisionz(newx, newz, g_door[2][1].getDoorOpen(), g_door[2][0].getPos().x, g_door[2][0].getPos().z, 0.04f) == 1) newz = g_eyeloc.z;
                        }
                    }

                    if (g_eyeloc.x != newx) g_eyeloc.x += direction.x * moveDistance; // eye 同步移動
                    if (g_eyeloc.z != newz) g_eyeloc.z += direction.z * moveDistance;

                    CCamera::getInstance().move(g_eyeloc);
                    mxView = CCamera::getInstance().getViewMatrix();
                    viewLoc = glGetUniformLocation(g_shadingProg, "mxView"); // 取得 view matrix 變數的位置
                    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mxView));
                    break;
                case 'E': //物件互動
                case 'e':
                    for (int i = 0; i < 6; i++) {
                        if (g_door[i][1].Collision(CCamera::getInstance().getViewLocation(), 1.0f) && g_door[i][1].getDoorActive()) {
                            std::cout << "OOPS" << std::endl;
                            g_door[i][1].setDoorAction();
                        }
                    }
                    if (g_key.getKeyActive() && !g_door[5][1].getDoorActive() && g_door[5][1].Collision(CCamera::getInstance().getViewLocation(), 1.0f)) {
                        g_door[5][1].setDoorActive(true);
                        lockdisable = true;
                    }
                    if (g_key.getKeyActive() && !g_door[1][1].getDoorActive() && g_door[1][1].Collision(CCamera::getInstance().getViewLocation(), 1.0f)) {
                        g_door[1][1].setDoorActive(true);
                        lockdisable2 = true;
                    }
                    if (g_key.Collision(CCamera::getInstance().getViewLocation().x, CCamera::getInstance().getViewLocation().z, 3.0f) && !g_key.getKeyActive()) {
                        g_key.setKeyActive();
                    };
                    if (g_sakula.Collision(CCamera::getInstance().getViewLocation().x, CCamera::getInstance().getViewLocation().z, 6.0f) && g_light[7].getisArrive() && g_light[8].getisArrive() && g_light[9].getisArrive()) {
                        flower_active = true;
                    }
                    break;
                }
            }
        }
    }
}