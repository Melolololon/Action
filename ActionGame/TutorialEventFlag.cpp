#include "TutorialEventFlag.h"

#include"Player.h"



TutorialEventFlag::TutorialEventFlag(const MelLib::Vector3& pos, const MelLib::Vector3& scale)
	:GameObject("TutorialEventFlag")
{
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetRadius(scale.x / 2);

	//modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));
	//modelObjects["main"].SetScale(sphereData[0].GetRadius() / 2);

	SetPosition(pos);
}

void TutorialEventFlag::Draw()
{
	// Šm”F—p
	//modelObjects["main"].Draw();
}

void TutorialEventFlag::Hit(const GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
	if (typeid(object) == typeid(Player))eraseManager = true;
}
