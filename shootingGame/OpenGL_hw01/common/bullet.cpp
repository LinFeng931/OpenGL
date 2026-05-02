#define GLM_ENABLE_EXPERIMENTAL
#include "initshader.h"
#include "wmhandler.h"
#include <glm/gtx/norm.hpp>
#include "CShaderPool.h"
#include "CBulletOne.h"
#include "CAirplane.h"
#include "villain.h"

extern GLuint g_shaderProg;
extern CAirplane g_airplane;

//子彈鏈結串列宣告
struct Bullet {
    CBulletOne bullet[2];
    Bullet* next_pointer;
    Bullet* last_pointer;
    int type;
    int v_idx;
    int account;
};
Bullet* player_bullet = NULL;
Bullet* pcurrent = NULL;
Bullet* phead = NULL;
int bullet_num = 1;

void PnewBullet(float posx, float posy) { //子彈鏈結串列
    if (player_bullet == NULL) {
        player_bullet = new Bullet;
        player_bullet->v_idx = 0;
        phead = player_bullet;
        player_bullet->next_pointer = NULL;
        player_bullet->last_pointer = NULL;
        player_bullet->account = 1;

        for (int i = 0; i < 2; i++) {
            player_bullet->bullet[i].setupVertexAttributes();
            player_bullet->bullet[i].setShaderID(g_shaderProg);

            if (g_airplane.getType() == 3) {
                player_bullet->type = 3;
                player_bullet->bullet[i].setPos(glm::vec3(posx + 0.5f * i, posy, 0.0f));
                player_bullet->bullet[i].setPos(glm::vec3(player_bullet->bullet[i].getPos().x - 0.25f, player_bullet->bullet[i].getPos().y, 0.0f));
            }

            else if (g_airplane.getType() == 2) {
                player_bullet->type = 2;
                player_bullet->bullet[i].setType(2);
                glm::vec3 v_pos;
                if (v_getIndex() == 4 && v_get(v_getIndex()).gun[0].getStatus() != -1 && v_get(v_getIndex()).gun[1].getStatus() != -1) {

                    for (int i = 0; i < 2; i++) {
                        if (!v_OK(4) || v_get(4).gun[i].getStatus() == -1) continue;
                        if (glm::distance2(g_airplane.getPos(), v_pos) > glm::distance2(g_airplane.getPos(), v_get(4).gun[i].getPos())) {
                            player_bullet->v_idx = i;
                            v_pos = v_get(4).gun[i].getPos();
                        }
                    }
                    player_bullet->bullet[i].cathcPos(v_get(player_bullet->v_idx).getPos()); // 給子彈一個目標座標
                    player_bullet->bullet[i].setPos(glm::vec3(posx, posy, 0.0f));
                }
                else {
                    v_pos = v_get(v_getIndex()).getPos();
                    for (int i = v_getIndex(); i < (v_getAmount() + v_getIndex()); i++) {
                        if (!v_OK(i) || v_get(i).getStatus() == -1) continue;
                        if (glm::distance2(g_airplane.getPos(), v_pos) > glm::distance2(g_airplane.getPos(), v_get(i).getPos())) {
                            player_bullet->v_idx = i;
                            v_pos = v_get(i).getPos();
                        }
                    }
                    player_bullet->bullet[i].cathcPos(v_get(player_bullet->v_idx).getPos()); // 給子彈一個目標座標
                    player_bullet->bullet[i].setPos(glm::vec3(posx, posy, 0.0f));
                }
            }

            else {
                player_bullet->type = 1;
                player_bullet->bullet[i].setPos(glm::vec3(posx, posy, 0.0f));
            }
        }
    }
    else {
        pcurrent = player_bullet;
        player_bullet = new Bullet;
        player_bullet->v_idx = 0;
        player_bullet->account = pcurrent->account + 1;
        pcurrent->next_pointer = player_bullet;
        player_bullet->next_pointer = NULL;
        player_bullet->last_pointer = pcurrent;

        for (int i = 0; i < 2; i++) {
            player_bullet->bullet[i].setupVertexAttributes();
            player_bullet->bullet[i].setShaderID(g_shaderProg);

            if (g_airplane.getType() == 3) {
                player_bullet->type = 3;
                player_bullet->bullet[i].setPos(glm::vec3(posx + 0.5f * i, posy, 0.0f));
                player_bullet->bullet[i].setPos(glm::vec3(player_bullet->bullet[i].getPos().x - 0.25f, player_bullet->bullet[i].getPos().y, 0.0f));
            }

            else if (g_airplane.getType() == 2) {
                player_bullet->type = 2;
                player_bullet->bullet[i].setType(2);
                glm::vec3 v_pos;
                if (v_getIndex() == 4 && (v_get(v_getIndex()).gun[0].getStatus() != -1 || v_get(v_getIndex()).gun[1].getStatus() != -1)) {
                    if (v_get(4).gun[0].getStatus() != -1) v_pos = v_get(4).gun[0].getPos();
                    else v_pos = v_get(4).gun[1].getPos();

                    for (int i = 0; i < 2; i++) {
                        if (glm::distance2(g_airplane.getPos(), v_pos) > glm::distance2(g_airplane.getPos(), v_get(4).gun[i].getPos())) {
                            if (v_get(4).gun[i].getStatus() != -1) {
                                player_bullet->v_idx = i;
                                v_pos = v_get(4).gun[i].getPos();
                            }    
                        }
                    }
                    player_bullet->bullet[i].cathcPos(v_get(player_bullet->v_idx).getPos()); // 給子彈一個目標座標
                    player_bullet->bullet[i].setPos(glm::vec3(posx, posy, 0.0f));
                }
                else if (v_getIndex() == 4) {
                    v_pos = v_get(4).getPos();
                    player_bullet->v_idx = 4;
                    
                    player_bullet->bullet[i].cathcPos(v_get(player_bullet->v_idx).getPos()); // 給子彈一個目標座標
                    player_bullet->bullet[i].setPos(glm::vec3(posx, posy, 0.0f));
                }
                else {
                    v_pos = v_get(v_getIndex()).getPos();
                    for (int i = v_getIndex(); i < (v_getAmount() + v_getIndex()); i++) {
                        if (glm::distance2(g_airplane.getPos(), v_pos) > glm::distance2(g_airplane.getPos(), v_get(i).getPos())) {
                            player_bullet->v_idx = i;
                            v_pos = v_get(i).getPos();
                        }
                    }
                    player_bullet->bullet[i].cathcPos(v_get(player_bullet->v_idx).getPos()); // 給子彈一個目標座標
                    player_bullet->bullet[i].setPos(glm::vec3(posx, posy, 0.0f));
                }
            }

            else {
                player_bullet->type = 1;
                player_bullet->bullet[i].setPos(glm::vec3(posx, posy, 0.0f));
            }
        }
    }

    if (player_bullet != NULL && player_bullet->account % 30 == 0 && phead != NULL) {
        for (int i = 0; i < 15 && phead != NULL; i++) {
            pcurrent = phead->next_pointer;
            if (phead) phead->last_pointer = nullptr;
            delete phead;
            phead = pcurrent;
        }
        if(player_bullet == nullptr) {
            do {
                player_bullet = pcurrent;
            } while (pcurrent->next_pointer != NULL);
        }
    }
    if (player_bullet != phead) {
        if (player_bullet->last_pointer == nullptr) {
            pcurrent = phead;
            while (pcurrent->next_pointer != NULL || pcurrent->next_pointer != player_bullet) {
                pcurrent = pcurrent->next_pointer;
            }    
            pcurrent->next_pointer = player_bullet;
            player_bullet->last_pointer = pcurrent;
            player_bullet->next_pointer = NULL;
        }
    }
    std::cout << player_bullet << " , " << player_bullet->last_pointer << " , " << player_bullet->next_pointer << std::endl;
}

void bullet_release() {
    while (phead != NULL) {
        pcurrent = phead->next_pointer;
        delete phead;
        phead = pcurrent;
    }
    player_bullet = NULL;
}

void bullet_collision() {
    pcurrent = phead;

    while (pcurrent != NULL) {
        Bullet* current = pcurrent;  // 保留目前的子彈
        pcurrent = pcurrent->next_pointer; // 預先移動到下一個

        for (int i = v_getIndex(); i < v_getAmount()+v_getIndex(); i++) {
            for (int j = 0; j < bullet_num; j++) {
                if (i == 4 && (v_get(i).gun[0].getStatus() != -1 || v_get(i).gun[1].getStatus() != -1)) {
                    for (int r = 0; r < 2; r++) {
                        if (v_get(i).gun[r].checkCollision(current->bullet[j].getPos(), current->bullet[j].getRadius())) {
                            // 若是 head，就要移動 head
                            if (current == phead) {
                                phead = current->next_pointer;
                                if (phead != NULL) {
                                    phead->last_pointer = NULL;
                                }
                            }
                            else {
                                if (current->last_pointer != NULL) current->last_pointer->next_pointer = current->next_pointer;

                                if (current->next_pointer != NULL) current->next_pointer->last_pointer = current->last_pointer;
                            }

                            // 如果剛好是最後一顆子彈，更新 player_bullet
                            if (player_bullet == current) {
                                player_bullet = NULL;
                            }

                            delete current;

                            std::cout << "Oops!!!" << std::endl;
                            v_get(i).gun[r].Flashing();
                        }
                    }
                    continue;
                }
                if (v_get(i).getStatus() == -1 || v_OK(i)!=true) continue;

                if (v_get(i).checkCollision(current->bullet[j].getPos(), current->bullet[j].getRadius())) {
                    // 若是 head，就要移動 head
                    if (current == phead) {
                        phead = current->next_pointer;
                        if (phead != NULL) phead->last_pointer = NULL;
                    }
                    else {
                        if (current->last_pointer != NULL) current->last_pointer->next_pointer = current->next_pointer;
                        if (current->next_pointer != NULL) current->next_pointer->last_pointer = current->last_pointer;
                    }

                     // 如果剛好是最後一顆子彈，更新 player_bullet
                     if (player_bullet == current) {
                            player_bullet = NULL;
                     }

                     delete current;

                     std::cout << "Oops!!!" << std::endl;
                      v_get(i).Flashing();
                }
            }   
        }
    }
}


void bullet_draw() {    
    pcurrent = phead;
    while (pcurrent != NULL) {
        for (int i = 0; i < 2; i++) pcurrent->bullet[i].draw();
        pcurrent = pcurrent->next_pointer;
    } 
}

void bullet_update(float dt) {
    pcurrent = phead;
    if (g_airplane.getType() == 3) bullet_num = 2;
    else bullet_num = 1;

    while (pcurrent != NULL) {
        for (int i = 0; i < 2; i++) { 
            if (g_airplane.getType() == 2 || pcurrent->type == 2) {
                if (pcurrent->v_idx == 4 && (v_get(4).gun[0].getStatus() != -1 || v_get(4).gun[1].getStatus() != -1)) pcurrent->bullet[i].cathcPos(v_get(4).gun[pcurrent->v_idx].getPos());
                else pcurrent->bullet[i].cathcPos(v_get(pcurrent->v_idx).getPos());
            }
            pcurrent->bullet[i].update(dt); 
        }
        pcurrent = pcurrent->next_pointer;
    }
}