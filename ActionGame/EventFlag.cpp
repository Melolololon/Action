#include "EventFlag.h"

void EventFlag::LoadResources()
{
	MelLib::ModelData::Load("Resources/Model/",true,"event");
}

EventFlag::EventFlag(const MelLib::Vector3& pos, const MelLib::Vector3& scale)
{
	SetPosition(pos);

	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(GetPosition());
	sphereDatas["main"][0].SetRadius(scale.x);

	//modelObjects["main"].Create(MelLib::ModelData::Get("event"));
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));
	modelObjects["main"].SetPosition(pos);
	modelObjects["main"].SetScale(scale);
}

void EventFlag::Draw()
{
	modelObjects["main"].Draw();
}
