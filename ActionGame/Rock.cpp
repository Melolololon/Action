#include "Rock.h"

void Rock::LoadResources()
{
	MelLib::ModelData::Load("Resources/Model/Object/Rock/Rock.obj", true, "rock1");
	MelLib::ModelData::Load("Resources/Model/Object/Rock/Rock2.obj", true, "rock2");
	MelLib::ModelData::Load("Resources/Model/Object/Rock/Rock3.obj", true, "rock3");
}

Rock::Rock(const MelLib::Vector3& pos, const MelLib::Vector3& angle, const MelLib::Vector3& scale,const int rockType)
	: StageObject(pos, "rock" + std::to_string(rockType))
{
	sphereDatas["main"].resize(1);

	sphereDatas["main"][0].SetPosition(pos);
	sphereDatas["main"][0].SetRadius(scale.x / 2.1);

}

