// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" 
#include "Vertex.h"
#include "Index.h"
#include "Shaders.h"
#include "Camera.h"
#include "Globals.h"
#include "Helper.h"
#include <conio.h>
#include <iostream>
#include <stdio.h>
#include <math.h>


GLuint vboId, vboQuad;
GLuint iboId;
GLuint textureHandle;
GLuint framebuffer, colorTex, depthTex;
Shaders myShaders, ppShaders;
int vertices, indices;

bool up = true;

GLfloat time = 0, ampFactor = 0.01;

Camera camera(Vector3(0.0f, 0.0f, 120.0f));

Matrix transformMatrix;

Matrix modelMatrix;
Matrix viewMatrix;
Matrix projectionMatrix;

int Init ( ESContext *esContext )
{
	glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);

	//triangle data (heap)
	FILE *pFile;
	fopen_s(&pFile, "../Resources/Models/Terrain.nfg", "r");
	fscanf_s(pFile, "NrVertices: %d", &vertices);
	Vertex *verticesData = new Vertex[vertices];
	for (int i = 0; i<vertices; i++) {
		fscanf_s(pFile, " %*d. pos:[%f, %f, %f]; norm:[%*f, %*f, %*f]; binorm:[%*f, %*f, %*f]; tgt:[%*f, %*f, %*f]; uv:[%f, %f]; ", &verticesData[i].pos.x, &verticesData[i].pos.y, &verticesData[i].pos.z, &verticesData[i].uv.x, &verticesData[i].uv.y);
	}
		
	//buffer object
	glGenBuffers(1, &vboId); //buffer object name generation
	glBindBuffer(GL_ARRAY_BUFFER, vboId); //buffer object binding
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices, verticesData, GL_STATIC_DRAW); //creation and initializion of buffer onject storage
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	fscanf_s(pFile, "NrIndices: %d", &indices);
	Index *indicesData = new Index[indices];
	for (int i = 0; i<indices/3; i++) {
		fscanf_s(pFile, " %*d. %u, %u, %u ", &indicesData[i].x, &indicesData[i].y, &indicesData[i].z);
	}
	
	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short)*indices, indicesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	fclose(pFile);
	
	Helper::loadTexture(&textureHandle, "../Resources/Textures/Rock.tga");
	Vector3 modelPivot = Helper::getModelPivot(vertices, verticesData);
	modelMatrix.SetIdentity();
	modelMatrix.SetTranslation(-modelPivot.x, -modelPivot.y, -modelPivot.z);
	modelMatrix = modelMatrix * modelMatrix.SetRotationX(90.0f);
	viewMatrix = camera.GetViewMatrix();
	projectionMatrix.SetPerspective(45.0f, float(Globals::screenWidth) / float(Globals::screenHeight), 0.1f, 500.0f);
	
	transformMatrix = modelMatrix * viewMatrix * projectionMatrix;

	delete []verticesData;
	delete []indicesData;

	//FBO
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glGenTextures(1, &colorTex);
	glBindTexture(GL_TEXTURE_2D, colorTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//NULL means reserve texture memory, but texels are undefined
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Globals::screenWidth, Globals::screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Vertex quadData[6];

	quadData[0].pos.x = -1.0f;  quadData[0].pos.y = 1.0f;  quadData[0].pos.z = 0.0f;
	quadData[1].pos.x = -1.0f;  quadData[1].pos.y = -1.0f;  quadData[1].pos.z = 0.0f;
	quadData[2].pos.x = 1.0f;  quadData[2].pos.y = -1.0f;  quadData[2].pos.z = 0.0f;
	quadData[3].pos.x = -1.0f;  quadData[3].pos.y = 1.0f;  quadData[3].pos.z = 0.0f;
	quadData[4].pos.x = 1.0f;  quadData[4].pos.y = -1.0f;  quadData[4].pos.z = 0.0f;
	quadData[5].pos.x = 1.0f;  quadData[5].pos.y = 1.0f;  quadData[5].pos.z = 0.0f;

	quadData[0].uv.x = 0.0f;	quadData[0].uv.y = 1.0f;
	quadData[1].uv.x = 0.0f;	quadData[1].uv.y = 0.0f;
	quadData[2].uv.x = 1.0f;	quadData[2].uv.y = 0.0f;
	quadData[3].uv.x = 0.0f;	quadData[3].uv.y = 1.0f;
	quadData[4].uv.x = 1.0f;	quadData[4].uv.y = 0.0f;
	quadData[5].uv.x = 1.0f;	quadData[5].uv.y = 1.0f;


	//buffer object
	glGenBuffers(1, &vboQuad); //buffer object name generation
	glBindBuffer(GL_ARRAY_BUFFER, vboQuad); //buffer object binding
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadData), quadData, GL_STATIC_DRAW); //creation and initializion of buffer onject storage
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//creation of shaders and program 
	return myShaders.Init("../Resources/Shaders/ShaderVS.vs", "../Resources/Shaders/ShaderFS.fs");

}

void Draw ( ESContext *esContext )
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(myShaders.program);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBindBuffer(GL_ARRAY_BUFFER, vboId);

	GLfloat* ptr = 0;
	if (myShaders.positionAttribute != -1) //attribute passing to shader, for uniforms use glUniform1f(time, deltaT); glUniformMatrix4fv( m_pShader->matrixWVP, 1, false, (GLfloat *)&rotationMat );
	{
		glEnableVertexAttribArray(myShaders.positionAttribute);
		glVertexAttribPointer(myShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), ptr);
	}

	if (myShaders.uvAttribute != -1) //attribute passing to shader, for uniforms use glUniform1f(time, deltaT); glUniformMatrix4fv( m_pShader->matrixWVP, 1, false, (GLfloat *)&rotationMat );
	{
		glEnableVertexAttribArray(myShaders.uvAttribute);
		glVertexAttribPointer(myShaders.uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ptr+3);
	}	
	
	if (myShaders.matrixTransform != -1) {
		glUniformMatrix4fv(myShaders.matrixTransform, 1, false, (GLfloat*)&transformMatrix);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

	unsigned short textureUnit = 0;
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glUniform1i(myShaders.textureUniform, textureUnit);

	glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_DEPTH_TEST);

	glUseProgram(ppShaders.program);
	glBindBuffer(GL_ARRAY_BUFFER, vboQuad);
	ptr = 0;
	if (ppShaders.positionAttribute != -1) //attribute passing to shader, for uniforms use glUniform1f(time, deltaT); glUniformMatrix4fv( m_pShader->matrixWVP, 1, false, (GLfloat *)&rotationMat );
	{
		glEnableVertexAttribArray(ppShaders.positionAttribute);
		glVertexAttribPointer(ppShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), ptr);
	}
	if (myShaders.uvAttribute != -1) //attribute passing to shader, for uniforms use glUniform1f(time, deltaT); glUniformMatrix4fv( m_pShader->matrixWVP, 1, false, (GLfloat *)&rotationMat );
	{
		glEnableVertexAttribArray(myShaders.uvAttribute);
		glVertexAttribPointer(ppShaders.uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ptr + 3);
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorTex);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update(ESContext *esContext, float deltaTime)
{
	viewMatrix = camera.GetViewMatrix();
	transformMatrix = modelMatrix * viewMatrix * projectionMatrix;
	time += deltaTime;
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	camera.ProcessKeyboard(CameraMovement(key));
}

void CleanUp()
{
	glDeleteBuffers(1, &vboId);
	glDeleteBuffers(1, &iboId);
	glDeleteTextures(1, &textureHandle);
	glDeleteBuffers(1, &vboQuad);
	glDeleteFramebuffers(1, &framebuffer);
}

int _tmain(int argc, _TCHAR* argv[])
{
	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Hello Model", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

	if (ppShaders.Init("../Resources/Shaders/PPShaderVS.vs", "../Resources/Shaders/PPShaderFS.fs") != 0)
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();

	//identifying memory leaks
	MemoryDump();
	printf("Press any key...\n");
	_getch();

	return 0;
}
