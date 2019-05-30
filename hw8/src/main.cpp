#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "shader.hpp"
#include "BezierCurve.hpp"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float lastX = 300.0f;
float lastY = 400.0f;

int step = 0;
bool finish = false;

vector<Point> ctrlPoints;
vector<Point> curvePoints;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	lastX = xpos;
	lastY = ypos;
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		curvePoints.clear();
		step = 0;
		finish = false;
		switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT: {
			Point p(lastX - SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f - lastY);
			ctrlPoints.push_back(p);
			break;
		}
		case GLFW_MOUSE_BUTTON_RIGHT: {
			if (!ctrlPoints.empty()) {
				ctrlPoints.pop_back();
			}
			break;
		}
		default:
			break;
		}
	}
}

float* createVertices(vector<Point> points, float R, float G, float B) {
	int point_num = points.size();
	float *vertices = new float[point_num * 6];
	for (int i = 0; i < point_num; i++) {
		vertices[i * 6 + 0] = (float)points[i].x / (float)SCR_WIDTH * 2;
		vertices[i * 6 + 1] = (float)points[i].y / (float)SCR_HEIGHT * 2;
		vertices[i * 6 + 2] = 0.0f;
		vertices[i * 6 + 3] = R;
		vertices[i * 6 + 4] = G;
		vertices[i * 6 + 5] = B;
	}
	return vertices;
}

void settingBuffer(unsigned int VAO, unsigned int VBO, int total, float *vertices) {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, total * sizeof(float), vertices, GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void clearBuffer(float *vertices) {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	delete vertices;
}

int main() {

	//初始化glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//创建窗口，如果创建失败打印错误信息
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "hellocube", NULL, NULL);
	if (!window) {
		printf("error on creating window");
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("error on initialize glad");
		exit(-1);
	}

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	Shader myShader("./shader.vs.glsl", "./shader.fs.glsl");

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		myShader.use();

		if (ctrlPoints.size() > 1) {

			if (finish == false) {
				BezierCurve::Bezier(ctrlPoints, curvePoints);
				finish = true;
			}
			int curvePoint_num = curvePoints.size();
			int total_curvePoint = curvePoint_num * 6;
			float *curveVertices = createVertices(curvePoints, 1.0f, 1.0f, 1.0f);

			settingBuffer(VAO, VBO, total_curvePoint, curveVertices);

			glPointSize(3.0f);
			glDrawArrays(GL_POINTS, 0, curvePoint_num);

			clearBuffer(curveVertices);

			if (step < PRECISION) {
				step += 5;
			}
			vector<Point> actionPoints;
			BezierCurve::getActionPoints(step, ctrlPoints, actionPoints);
			int actionPoint_num = actionPoints.size();
			int total_actionPoint = actionPoint_num * 6;
			float *actionVertices = createVertices(actionPoints, 0.8f, 0.8f, 0.8f);

			settingBuffer(VAO, VBO, total_actionPoint, actionVertices);

			glPointSize(8.0f);
			glDrawArrays(GL_POINTS, 0, actionPoint_num);

			glPointSize(1.0f);	
			int current = 0;
			int count = ctrlPoints.size() - 1;
			while (count > 1) {
				glDrawArrays(GL_LINE_STRIP, current, count);
				current += count;
				count--;
			}
			clearBuffer(actionVertices);
		}

		int ctrlPoint_num = ctrlPoints.size();
		int total_ctrlPoint = ctrlPoint_num * 6;
		float *ctrlPointVertices = createVertices(ctrlPoints, 0.8f, 0.8f, 0.8f);

		settingBuffer(VAO, VBO, total_ctrlPoint, ctrlPointVertices);

		glPointSize(15.0f);	
		glDrawArrays(GL_POINTS, 0, ctrlPoint_num);

		glPointSize(1.0f);
		glDrawArrays(GL_LINE_STRIP, 0, ctrlPoint_num);

		clearBuffer(ctrlPointVertices);

		glfwSwapBuffers(window);
	}

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}