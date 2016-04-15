#include "stdafx.h"
#include "Helper.h"

Vector3 Helper::getModelPivot(long vertices, Vertex* verticesData) {
	float minX = verticesData[0].pos.x, maxX = verticesData[0].pos.x;
	float minY = verticesData[0].pos.y, maxY = verticesData[0].pos.y;
	float minZ = verticesData[0].pos.z, maxZ = verticesData[0].pos.z;

	for (int i = 1; i < vertices; i++) {
		if (minX > verticesData[i].pos.x) {
			minX = verticesData[i].pos.x;
		}
		if (maxX < verticesData[i].pos.x) {
			maxX = verticesData[i].pos.x;
		}
		if (minY > verticesData[i].pos.y) {
			minY = verticesData[i].pos.y;
		}
		if (maxY < verticesData[i].pos.y) {
			maxY = verticesData[i].pos.y;
		}
		if (minZ > verticesData[i].pos.z) {
			minZ = verticesData[i].pos.z;
		}
		if (maxZ < verticesData[i].pos.z) {
			maxZ = verticesData[i].pos.z;
		}
	}
	return Vector3((minX + maxX) / 2.0f, (minY + maxY) / 2.0f, (minZ + maxZ) / 2.0f);
}

void Helper::loadTexture(GLuint *textureHandle, char* pathToFile) {
	glGenTextures(1, textureHandle);
	glBindTexture(GL_TEXTURE_2D, *textureHandle);
	int width, height, bpp;
	char* bufferTGA = LoadTGA(pathToFile, &width, &height, &bpp);

	if (bpp == 24) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bufferTGA);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bufferTGA);
	}
	delete[]bufferTGA;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
}