#include "CShaderPool.h"
#include "CStar.h"

extern GLuint g_shaderProg;

struct Stars {
	Stars* next;
	CStar star;
};

Stars* head = NULL;
Stars* pointer = NULL;
Stars* tail = NULL;
float timer = 0.0f;

void background_update(float dt) {
	timer += dt;
	float pos_x = (rand() % 500) * 0.01f * (rand() % 3 - 0.9f);

	//·s¼W
	if (head == NULL && timer> 0.2f) {
		head = new Stars;
		head->next = NULL;
		pointer = head;
		tail = head;
		timer = 0.0f;
		
		head->star.setupVertexAttributes();
		head->star.setShaderID(g_shaderProg);
		head->star.setPos(glm::vec3(pos_x,5.0f, 0.0f));

		switch (rand()%4)
		{
		case 0:
			head->star.setColor(glm::vec3(0.4f, 0.5f, 0.9f));
			head->star.setScale(glm::vec3(2.0f));
			break;
		case 1:
			head->star.setColor(glm::vec3(0.3f, 0.5f, 0.6f));
			head->star.setScale(glm::vec3(1.6f));
			break;
		case 2:
			head->star.setColor(glm::vec3(0.5f, 0.4f, 0.6f));
			head->star.setScale(glm::vec3(1.3f));
			break;				
		case 3:
			head->star.setColor(glm::vec3((0.1f, 0.2f, 0.6f)));
			break;
		default:
			break;
		}
	}
	else if (head != NULL && timer > 0.5f) {
		pointer = tail;
		tail = new Stars;
		tail->next = NULL;
		pointer->next = tail;
		timer = 0.0f;

		tail->star.setupVertexAttributes();
		tail->star.setShaderID(g_shaderProg);
		tail->star.setPos(glm::vec3(pos_x,5.0f , 0.0f));

		switch (rand() % 4)
		{
		case 0:
			pointer->star.setColor(glm::vec3(0.4f, 0.5f, 0.9f));
			pointer->star.setScale(glm::vec3(2.0f));
			break;
		case 1:
			pointer->star.setColor(glm::vec3(0.3f, 0.5f, 0.6f));
			pointer->star.setScale(glm::vec3(1.6f));
			break;
		case 2:
			pointer->star.setColor(glm::vec3(0.5f, 0.4f, 0.6f));
			pointer->star.setScale(glm::vec3(1.3f));
			break;
		case 3:
			pointer->star.setColor(glm::vec3(0.1f, 0.2f, 0.6f));
			break;
		default:
			break;
		}
	}
	
	pointer = head;
	while (pointer != NULL) {
		pointer->star.update(dt);
		pointer = pointer->next;
	}

	//§R°£
	if (head != NULL) {
		if (head->star.getPos().y < -6.0f) {
			pointer = head->next;
			delete head;
			head = pointer;
		}
	}
}

void background_draw() {
	pointer = head;
	while (pointer != NULL) {
		pointer->star.draw();
		pointer = pointer->next;
	}
}

void background_release() {
	while (head != NULL) {
		pointer = head->next;
		delete head;
		head = pointer;
	}
	tail = nullptr;
}