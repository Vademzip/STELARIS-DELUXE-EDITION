#include "LightingShader.h"

extern Camera3D camera;

LightingShader::LightingShader()
{
    raylibShader = LoadShader(TextFormat("resources/shaders/glsl%i/lighting.vs", 330),
        TextFormat("resources/shaders/glsl%i/lighting.fs", 330));

    raylibShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(raylibShader, "viewPos");
    // NOTE: "matModel" location name is automatically assigned on shader loading, 
    // no need to get the location again if using that uniform name
    //shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(shader, "matModel");

    // Ambient light level (some basic lighting)
    int ambientLoc = GetShaderLocation(raylibShader, "ambient");
    float s[4] = { 10.5f, 1.5f, 1.5f, 1.0f };
    SetShaderValue(raylibShader, ambientLoc, s, SHADER_UNIFORM_VEC4);

}
void LightingShader::Update()
{
	float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
	SetShaderValue(raylibShader, raylibShader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
}