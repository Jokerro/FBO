#pragma once

#include "../Utilities/utilities.h"
#include "Vertex.h"

class Helper {

public:
	static Vector3 getModelPivot(long, Vertex*);
	static void loadTexture(GLuint*, char*);

};