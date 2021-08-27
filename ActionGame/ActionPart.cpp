#include "ActionPart.h"

#include"DirectionalLight.h"

#include"GameObjectManager.h"

#include"Player.h"

#include"Ground.h"

void ActionPart::Initialize()
{
	MelLib::DirectionalLight::Get().SetDirection(MelLib::Vector3(0, -1, 0));

	Ground::LoadResource();
	
	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<Player>(0));

	
	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<Ground>());

}

void ActionPart::Update()
{
	MelLib::GameObjectManager::GetInstance()->Update();
}

void ActionPart::Draw()
{
	MelLib::GameObjectManager::GetInstance()->Draw();
}

void ActionPart::Finalize()
{
}

MelLib::Scene* ActionPart::GetNextScene()
{
	return new ActionPart();
}
