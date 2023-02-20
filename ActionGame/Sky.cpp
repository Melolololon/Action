#include "Sky.h"

Sky::Sky()
{
	MelLib::ModelData::Load("Resources/Model/Sky/Sky.obj", false, "Sky");
	modelObject.Create(MelLib::ModelData::Get("Sky"), "Sky");
}

void Sky::Draw()
{
	modelObject.Draw();
}
