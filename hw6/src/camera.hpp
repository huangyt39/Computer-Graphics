#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Camera {
public:
	Camera() {
		Position = glm::vec3(0.0f, 0.0f, 3.0f);
		Front = glm::vec3(0.0f, 0.0f, -1.0f);
		WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	}

	glm::mat4 GetViewMatrix() {
		return glm::lookAt(Position, Position + Front, Up);
	}
	void moveForward(GLfloat const distance) {
		Position += Front * distance;
	}
	void moveBack(GLfloat const distance) {
		Position -= Front * distance;
	}
	void moveRight(GLfloat const distance) {
		Position += Right * distance;
	}
	void moveLeft(GLfloat const distance) {
		Position -= Right * distance;
	}

	void rotate(GLfloat const yaw, GLfloat const pitch) {
		Yaw += yaw;
		Pitch += pitch;
		float radian_yaw = glm::radians(Yaw), radian_pitch = glm::radians(Pitch);
		Pitch = Pitch > 89.0f ? 89.0f : (Pitch < -89.0f ? -89.0f : Pitch);
		glm::vec3 front = glm::vec3(cos(radian_yaw) * cos(radian_pitch), sin(radian_pitch), sin(radian_yaw) * cos(radian_pitch));
		Front = glm::normalize(front);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

	glm::vec3 getPosition() {
		return Position;
	}

private:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw = -90.0f;
	float Pitch = 0.0f;
};

#endif // !_CAMERA_H_