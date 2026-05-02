#include <iostream>

#include "wmhandler.h"
#include "arcball.h"
#include "CBulletTwo.h"
#include "CAirplane.h"
#include "CBulletOne.h"
#include "CShield.h"
#include "bullet.h"
#include "villain.h"

extern Arcball g_arcball;

extern CBulletTwo g_bullet2;
extern CAirplane g_airplane; //多階層中心
extern CBulletOne g_bullet;
extern CShield player_shield; //第二層

extern GLfloat g_viewScale;
extern bool g_bRotating;
extern bool g_bMoving;

// 滑鼠按鈕按下後 callback function(回呼函式) ---------------
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    // Arcball* arcball = static_cast<Arcball*>(glfwGetWindowUserPointer(window));
    g_arcball.onMouseButton(button, action, xpos, ypos);
    //std::cout << "button = " << button << "action = " << action << "mods = " << mods << std::endl;
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            if (g_airplane.getStatus() != 0) {
                if (g_airplane.getType() == 2) {
                    if (g_airplane.getTimer() >= 2.0f) {
                        PnewBullet(g_airplane.getPosX(), g_airplane.getPosY());
                        g_airplane.resetTimer();
                    }
                }
                else PnewBullet(g_airplane.getPosX(), g_airplane.getPosY());
            }
        }
    }
}
// ---------------------------------------------------------------------------------------

// 滑鼠在指定視窗上移動時的 callback function (回呼函式)
void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
  
   // g_arcball.onCursorMove(xpos, ypos, width, height);
    float normalizedX = (xpos / width) * 2.0f - 1.0f;
    float normalizedY = 1.0f - (ypos / height) * 2.0f;

    //std::cout << "x = " << normalizedX << "y = " << normalizedY << std::endl;
    float mouseScale = 4.0f; //控制靈敏度
    normalizedX = ((xpos / width) * 2.0f - 1.0f) * mouseScale;
    normalizedY = (1.0f - (ypos / height) * 2.0f) * mouseScale;

    if (g_airplane.getStatus() != 0) {
        g_airplane.setPos(glm::vec3(normalizedX, -3.0f, 0.0f));
        //if (player_shield.getStatus() == 2) player_shield.setPos(g_airplane.getPos());
        //else if (player_shield.getStatus() == 1) player_shield.setPos(glm::vec3(g_airplane.getPos().x - 1.0f, g_airplane.getPos().y, 0.0f));
    }
}
// ---------------------------------------------------------------------------------------
// 
// 當視窗大小改變的 callback function
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
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
    switch (key)
    {
        case GLFW_KEY_ESCAPE:
            if (action == GLFW_PRESS) { glfwSetWindowShouldClose(window, true); }
            break;

        case GLFW_KEY_SPACE:
             if (action == GLFW_PRESS) {       
                 if (player_shield.getStatus() == 1) {player_shield.setStatus(2); }
             }
            break;
        default: // 針對英文字母大小寫進行處理
            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                // 檢查 Shift 鍵(左右兩邊各一個)是否被按下
                bool isShiftPressed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ||
                                      (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);
                // 判斷字母鍵的大小寫，預設使用鍵盤是沒有按下 CAPS 鍵
                if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
                    char letter = (isShiftPressed) ? ('A' + (key - GLFW_KEY_A)) : ('a' + (key - GLFW_KEY_A));
                    //std::cout << "key = " << letter << std::endl;

                    switch (letter) {
                    case 'S':
                    case 's':
                        //g_bullet2.setScale(glm::vec3(0.25f, 0.25f, 1.0f));
                        break;
                    case 'L':
                    case 'l':
                        //g_bullet2.setScale(glm::vec3(1.2f, 1.2f, 1.0f));
                        break;
                    }
                }   
            }
        break;
    }
}