#pragma once
#include "raylib.h"
class GenericShader
{
protected:
	Shader raylibShader;

public:
	virtual void Update() = 0;
	Shader rShader();
	GenericShader();

};

