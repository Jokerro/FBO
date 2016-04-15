#pragma once
#include "../Utilities/utilities.h"

enum CameraMovement {
	FORWARD = 'W',
	BACKWARD = 'S',
	RIGHT = 'D',
	LEFT = 'A'
};

class Camera
{
public:
	
	Camera(Vector3 position = Vector3(0.0f, 0.0f, -1.0f), Vector3 up = Vector3(0.0f, 1.0f, 0.0f));

	Matrix GetViewMatrix();

	void ProcessKeyboard(CameraMovement direction);

private:
	Vector3 worldUp;
	Vector3 position;
	Vector3 front;
	Vector3 right;
	GLfloat MovementSpeed;

	Matrix LookAt(Vector3, Vector3, Vector3);

};