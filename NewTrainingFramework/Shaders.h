#pragma once
#include "../Utilities/utilities.h"

class Shaders 
{
public:
	GLuint program, vertexShader, fragmentShader;
	char fileVS[260];
	char fileFS[260];

	GLint positionAttribute;
	GLint uvAttribute, matrixTransform;
	GLint textureUniform;
	GLint timeUniform;
	GLint ampFactorUniform;

	int Init(char * fileVertexShader, char * fileFragmentShader);
	~Shaders();
};