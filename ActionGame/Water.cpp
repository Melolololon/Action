#include "Water.h"

void Water::LoadResources()
{
	MelLib::ModelData::Load("Resources/Model/Stage/Water/Water.obj", false, "Water");
}

Water::Water()
	:GameObject("Water")
{
	modelObjects["main"].Create(MelLib::ModelData::Get("Water"),"Water");
}

void Water::Initialize()
{
}

void Water::Update()
{
}

void Water::Draw()
{
	AllDraw();
}

std::shared_ptr<MelLib::GameObject> Water::GetNewPtr()
{
	return std::make_shared<Water>();
}
