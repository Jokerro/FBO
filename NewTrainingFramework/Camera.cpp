#pragma once

#include "stdafx.h"
#include "Camera.h"

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

const GLfloat SPEED = 0.1f;

Camera::Camera(Vector3 position, Vector3 up) : front(Vector3(0.0f, 0.0f, -1.0f * sgn(position.z))), MovementSpeed(SPEED)
{	
	this->position = position;
	this->worldUp = up;
}

Matrix Camera::LookAt(Vector3 cameraPos, Vector3 target, Vector3 up) {

	Vector3 cameraFront = (cameraPos - target).Normalize();
	Vector3 cameraRight = up.Cross(cameraFront).Normalize();
	Vector3 cameraUp = cameraFront.Cross(cameraRight);

	Matrix result;
	result.SetIdentity();

	result.m[0][0] = cameraRight.x;	result.m[1][0] = cameraRight.y;	result.m[2][0] = cameraRight.z;
	result.m[0][1] = cameraUp.x;	result.m[1][1] = cameraUp.y;	result.m[2][1] = cameraUp.z;
	result.m[0][2] = cameraFront.x;	result.m[1][2] = cameraFront.y;	result.m[2][2] = cameraFront.z;

	result.m[3][0] = -cameraRight.Dot(cameraPos);
	result.m[3][1] = -cameraUp.Dot(cameraPos);
	result.m[3][2] = -cameraFront.Dot(cameraPos);

	return result;
}

Matrix Camera::GetViewMatrix()
{
	return LookAt(position, position + front, worldUp);
}

void Camera::ProcessKeyboard(CameraMovement direction)
{
	GLfloat velocity = this->MovementSpeed;
	if (direction == FORWARD)
		position += front * velocity;
	if (direction == BACKWARD)
		position -= front * velocity;
	if (direction == LEFT)
		position -= front.Cross(worldUp).Normalize() * velocity;
	if (direction == RIGHT)
		position += front.Cross(worldUp).Normalize() * velocity;
}

