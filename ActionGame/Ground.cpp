#include "Ground.h"

#include"Game.h"

#include<ModelData.h>


const MelLib::Vector2 Ground::GROUND_SIZE = MelLib::Vector2(50, 50);

Ground::Ground()
{
	position = 0;


	material.Create(MelLib::PipelineState::GetDefaultDrawData(MelLib::PipelineStateType::MODEL));
	material.SetTexture(MelLib::Texture::Get("ground"));

	//モデル用意
	//板ポリをセットして生成
	model.Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), nullptr);
	model.SetMaterial(&material, 0);
	model.SetAngle(MelLib::Vector3(90.0f, 0.0f, 0.0f));
	model.SetScale(GROUND_SIZE.ToVector3());
	model.SetPosition(position);

	boardData.resize(1);
	boardData[0].SetAngle(MelLib::Vector3(90, 0, 0));
	boardData[0].SetSize(GROUND_SIZE);
	boardData[0].SetPosition(0);
}

void Ground::Update()
{
}

void Ground::Draw()
{
	model.Draw();
}

void Ground::Hit(const GameObject* const object, const MelLib::ShapeType3D collisionType, const int arrayNum, const MelLib::ShapeType3D hitObjColType, const int hitObjArrayNum)
{
}

void Ground::LoadResource()
{
	const std::string texturePath = Game::GetInstance()->GetPath(Game::ResourcePathType::TEXTURE);
	MelLib::Texture::Load(texturePath + "ground.png","ground");
}
